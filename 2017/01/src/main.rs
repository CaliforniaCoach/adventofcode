use std::fs;

fn main() {
    let chars: Vec<char> = fs::read_to_string("input.txt").unwrap().trim().chars().collect();

    let mut sum = 0;

    for i in 1..chars.len() {
        if chars[i - 1] == chars[i] {
            sum += chars[i].to_digit(10).unwrap();
        }
    }
    if chars.first().unwrap() == chars.last().unwrap() {
        sum += chars.first().unwrap().to_digit(10).unwrap();
    }

    println!("{sum}");

    // Part 2
    sum = 0;
    for i in 0..chars.len() {
        if chars[i] == chars[(i + chars.len() / 2) % chars.len()] {
            sum += chars[i].to_digit(10).unwrap();
        }
    }
    println!("{sum}");
}
