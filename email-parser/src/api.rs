use std::fs::File;
use std::io::{Cursor,Result,Read};
use std::path::PathBuf;

use mail_parser::Message;
use zip_extract;

const TEMP_PATH: &str = "/tmp/vsmtp";

pub fn test(email: String) -> Result<()> {
    let bytes = email.into_bytes();
    let message = Message::parse(&bytes).unwrap();
    let attatchment = message.attachment(0).unwrap();
    let attatchment_bytes = attatchment.contents();

    let target = PathBuf::from(TEMP_PATH);
    zip_extract::extract(Cursor::new(attatchment_bytes), &target, true).unwrap();

    let mut export_file = File::open(format!("{TEMP_PATH}/export.xml"))?;
    let mut export_data = String::new();

    export_file.read_to_string(&mut export_data)?;

    // TODO export xml data
    // TODO format request
    // TODO send request

    print!("{}", export_data);

    Ok(())
}
