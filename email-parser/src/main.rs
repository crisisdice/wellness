use std::fs::File;
use std::io::Result;
use std::io::prelude::*;
use std::io::Cursor;
use std::path::PathBuf;

use mail_parser::*;
use zip_extract;

fn main() -> Result<()> {
    // TODO watch for fs changes
    // FIXME error handling and paths
    let mut email = File::open("./emails/hde.eml")?;
    let mut email_buffer = Vec::new();

    email.read_to_end(&mut email_buffer)?;

    let message = Message::parse(&email_buffer).unwrap();
    let attatchment = message.attachment(0).unwrap();
    let attatchment_bytes = attatchment.contents();

    // File::create("./out.zip")?.write(attatchment_bytes)?;

    // if (!zip) {}

    let target = PathBuf::from("./out");
    zip_extract::extract(Cursor::new(attatchment_bytes), &target, true).unwrap();

    // TODO run req.sh

    Ok(())
}


