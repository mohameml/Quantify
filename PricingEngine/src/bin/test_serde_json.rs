use serde_json::Value;
use std::fs;

fn main() {
    // Step 1 : read the file
    let data = fs::read_to_string("data.json").expect("Impossible de lire le fichier");

    // Step 2 : parse the file to a Value from serde_json
    let v: Value = serde_json::from_str(&data).expect("JSON invalide");

    println!("v is {}", v);

    // Step 3 : Read the value from key
    let val = v.get("cle").unwrap().as_i64().unwrap();

    println!("val is : {}", val);
}
