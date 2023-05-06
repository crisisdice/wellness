use std::io::Cursor;
use std::path::PathBuf;

use mail_parser::Message;
use zip_extract;

use rhai_dylib::rhai::plugin::{
    mem, Dynamic, FnAccess, FnNamespace, ImmutableString, NativeCallContext, PluginFunction,
    RhaiResult, TypeId,
};

use rhai_dylib::rhai::Module;

const TMP_PATH: &str = "/tmp/vsmtp";

// The plugin API from rhai can be used to create your plugin API.
#[rhai_dylib::rhai::plugin::export_module]
pub mod wellness_plugin {
    /// Printing to the console using Rust.
    #[rhai_fn(global)]
    pub fn print_stuff(email: String) {
        let bytes = email.into_bytes();
        let message = Message::parse(&bytes).unwrap();
        let attatchment = message.attachment(0).unwrap();
        let attatchment_bytes = attatchment.contents();

        let target = PathBuf::from(TMP_PATH);
        zip_extract::extract(Cursor::new(attatchment_bytes), &target, true).unwrap();
    }
}
