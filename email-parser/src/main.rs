pub mod api;

use std::fs::File;
use std::io::Result;
use std::io::prelude::*;

fn main() -> Result<()> {
    // TODO pass email object to plugin
    // FIXME error handling and paths
    let mut email = File::open("./src/emails/hde.eml")?;
    let mut test = String::new();

    email.read_to_string(&mut test)?;

    api::test(test)?;

    Ok(())
}


