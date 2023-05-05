//use std::io::Cursor;
//use std::path::PathBuf;
//use std::io::Result;

use mail_parser::Message;
//use zip_extract;

use rhai_dylib::rhai::plugin::{
    mem, Dynamic, FnAccess, FnNamespace, ImmutableString, NativeCallContext, PluginFunction,
    RhaiResult, TypeId,
};

use rhai_dylib::rhai::Module;

// The plugin API from rhai can be used to create your plugin API.
#[rhai_dylib::rhai::plugin::export_module]
pub mod wellness_plugin {
    /// Printing to the console using Rust.
    #[rhai_fn(global)]
    //pub fn print_stuff(email: String) -> Result<()> {
    pub fn print_stuff(email: String) {
        let bytes = email.into_bytes();
        let message = Message::parse(&bytes).unwrap();
        // let attatchment = message.attachment(0).unwrap();
        // let attatchment_bytes = attatchment.contents();

        println!("{:?}", message.from());

        // let target = PathBuf::from("/out");
        // zip_extract::extract(Cursor::new(attatchment_bytes), &target, true).unwrap();
    }
}
