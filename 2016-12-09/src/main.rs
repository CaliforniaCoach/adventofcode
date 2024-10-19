use std::fs;
use regex::Regex;

fn main() {
    let content = fs::read_to_string("input.txt").unwrap();
    let line = content.lines().nth(0).unwrap();
    let mut decompressed = String::new();

    let regex = Regex::new(r"^\((\d+)x(\d+)\)").unwrap();

    let mut i = 0;
    while i < line.len() {
        if let Some(matches) = regex.captures(&line[i..]) {
            let num_chars: usize = (&matches[1]).parse().unwrap();
            let repeat: i32 = (&matches[2]).parse().unwrap();
            i += matches.get(0).unwrap().len();
            for _ in 0..repeat {
                decompressed.push_str(&line[i..i + num_chars]);
            }
            i += num_chars;
        } else {
            decompressed.push(line.chars().nth(i).unwrap());
            i += 1;
        }
    }
    let length = decompressed.len();
    println!("{length}");

    // Part 2

    // (7x2)(2x2)AB -> (2x2)AB(2x2)AB -> ABABABAB

    let length2 = count(line);
    println!("{length2}");
}


fn count(text: &str) -> usize {
    let regex2 = Regex::new(r"\((\d+)x(\d+)\)").unwrap();
    match regex2.captures(&text) {
        Some(matches) => {
            let num_chars: usize = (&matches[1]).parse().unwrap();
            let repeat: usize = (&matches[2]).parse().unwrap();
            let start = matches.get(0).unwrap().start();
            let end = matches.get(0).unwrap().end();
            start + repeat * count(&text[end..end + num_chars]) + count(&text[end + num_chars..])
        }
        _ => text.len()
    }
}
