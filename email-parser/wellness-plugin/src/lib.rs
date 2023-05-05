use rhai::plugin::*;

#[export_module]
mod wellness_plugin {
    // Build your module here.

    pub fn test() {
        print!("hello world");
    }
}

/// Export the wellness_plugin module.
#[no_mangle]
pub extern "C" fn module_entrypoint() -> rhai::Shared<rhai::Module> {

    // The seed must be the same as the one used in the program that will
    // load this module.
    rhai::config::hashing::set_ahash_seed(Some([1, 2, 3, 4])).unwrap();

    rhai::exported_module!(wellness_plugin).into()
}
