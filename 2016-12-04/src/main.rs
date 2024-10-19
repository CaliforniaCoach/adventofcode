use std::fs;
use std::collections::HashMap;
use std::collections::HashSet;
use std::collections::BTreeMap;


enum Part {
    Name,
    SectorId,
    Checksum
}

fn rotate(text: &String, rot: u32) -> String {
    let mut result = String::new();
    let a_val = u32::from('a');
    for chr in text.chars() {
        result.push(
            match chr {
                '-' => ' ',
                _ => char::from_u32((u32::from(chr) - a_val + rot) % 26 + a_val).unwrap()
            }
        )
    }
    result
}

fn main() {
    let content = fs::read_to_string("input.txt").unwrap();
    let search_word = "north";
    let mut sum = 0;
    'line: for line in content.lines() {
        let mut room_name = String::new();
        let mut sector_id = 0;
        let mut part = Part::Name;
        let mut hist: HashMap<char, i32> = HashMap::new();
        let mut hist_inverted: BTreeMap<i32, HashSet<char>> = BTreeMap::new();
        for character in line.chars() {
            if character.is_numeric() {
                part = Part::SectorId;
            } else if character == '[' {
                part = Part::Checksum;
                hist.remove(&'-');
                for (char, count) in &hist {
                    hist_inverted.entry(*count)
                        .and_modify(|chars| { chars.insert(*char); })
                        .or_insert(HashSet::from([*char]));
                }
                /*
                println!("{line}");
                for (key, vals) in &hist_inverted {
                    println!("{key}");
                    for val in vals {
                        println!(" -> {val}");
                    }
                }
                */
                continue;
            }
            
            match part {
                Part::Name => {
                    hist.entry(character).and_modify(|count| *count += 1).or_insert(1);
                    room_name.push(character);
                }

                Part::SectorId => 
                    sector_id = sector_id * 10 + character.to_digit(10).unwrap(),

                Part::Checksum => 
                    if character != ']' {
                        if let Some(mut entry) = hist_inverted.last_entry() {
                            if !entry.get_mut().remove(&character) {
                                continue 'line;
                            }
                            if entry.get().is_empty() {
                                entry.remove();
                            }
                        }
                    } else {
                        sum += sector_id;
                        let decrypted_name = rotate(&room_name, sector_id).trim().to_owned();
                        if decrypted_name.contains(search_word) {
                            println!("{decrypted_name} -> number {sector_id}");
                        }
                    }
            }
        }
    }
    println!("Sum of all real room sector IDs: {sum}");

    // part 2
}
