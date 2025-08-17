use std::fs;

const VOWELS: [char; 5] = ['a', 'e', 'i', 'o', 'u'];
const INVALIDS: [&str; 4] = ["ab", "cd", "pq", "xy"];

fn count_vowels(line: &str) -> u32 {
    let mut count = 0;
    for chr in line.chars() {
        if VOWELS.contains(&chr) {
            count += 1;
        }
    }
    count
}

fn has_doubles(line: &str) -> bool {
    let mut chr1 = line.chars().nth(0).unwrap();
    for i in 1 .. line.len() {
        let chr2 = line.chars().nth(i).unwrap();
        if chr1 == chr2 {
            return true;
        }
        chr1 = chr2;
    }
    false
}

fn contains_invalids(line: &str) -> bool {
    for invalid_substr in INVALIDS {
        if line.contains(invalid_substr) {
            return true;
        }
    }
    false
}

fn pair_appears_twice(line: &str) -> bool {
    for i in 0 .. line.len() - 3 {
        let pair = &line[i .. i + 2];
        if line[i + 2 ..].contains(pair) {
            return true;
        }
    }
    false
}

fn contains_xyx_pattern(line: &str) -> bool {
    for i in 0 .. line.len() - 2 {
        if line.chars().nth(i) == line.chars().nth(i + 2) {
            return true;
        }
    }
    false
}

fn main() {
    let mut nice1 = 0;
    let mut nice2 = 0;
    for line in fs::read_to_string("input.txt").unwrap().lines() {
        if count_vowels(line) >= 3 && has_doubles(line) && !contains_invalids(line) {
            nice1 += 1;
        }
        if pair_appears_twice(line) && contains_xyx_pattern(line) {
            nice2 += 1;
        }
    }
    println!("Nice ones part one: {nice1}");
    println!("Nice ones part two: {nice2}");
}
