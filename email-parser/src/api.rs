use std::io::Cursor;
use std::path::PathBuf;
use std::io::Result;

use mail_parser::Message;
use zip_extract;

pub fn test(email: String) -> Result<()> {
    let bytes = email.into_bytes();
    let message = Message::parse(&bytes).unwrap();
    let attatchment = message.attachment(0).unwrap();
    let attatchment_bytes = attatchment.contents();

    let target = PathBuf::from("./out");
    zip_extract::extract(Cursor::new(attatchment_bytes), &target, true).unwrap();
    Ok(())
}
