use std::fs;
use std::collections::HashMap;


fn main() {
    let content = fs::read_to_string("input.txt").unwrap();
    let mut hist: Vec<HashMap<char, i32>> = Vec::new();
    for line in content.lines() {
        for (i, chr) in line.chars().enumerate() {
            if hist.len() <= i {
                hist.push(HashMap::new());
            }
            hist.get_mut(i).unwrap().entry(chr).and_modify(|count| *count += 1).or_insert(1);
        }
    }

    let mut max_str = String::new();
    let mut min_str = String::new();
    for place in hist {
        max_str.push(*place.iter().max_by(|a, b| a.1.cmp(&b.1)).unwrap().0);
        min_str.push(*place.iter().min_by(|a, b| a.1.cmp(&b.1)).unwrap().0);
    }
    println!("{max_str}");
    println!("{min_str}");
}
