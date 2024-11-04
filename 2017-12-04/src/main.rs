use std::fs;
use std::collections::HashSet;

fn main() {
    let mut count = 0;
    'line: for line in fs::read_to_string("input.txt").unwrap().lines() {
        let mut words: HashSet<&str> = HashSet::new();
        for word in line.split_whitespace() {
            if words.contains(word) {
                continue 'line
            } else {
                words.insert(word);
            }
        }
        count += 1;
    }
    println!("{count}");


    // Part 2

    count = 0;
    'line: for line in fs::read_to_string("input.txt").unwrap().lines() {
        let mut words: HashSet<&str> = HashSet::new();
        for word in line.split_whitespace() {
            for word2 in &words {
                if is_anagram(word, word2) {
                    continue 'line
                }
            }
            words.insert(word);
        }
        count += 1;
    }
    println!("{count}");

    println!("{}", is_anagram("abc", "cba"));
}


fn is_anagram(text1: &str, text2: &str) -> bool {
    let mut sorted1 = text1.chars().collect::<Vec<char>>();
    let mut sorted2 = text2.chars().collect::<Vec<char>>();

    sorted1.sort();
    sorted2.sort();

    sorted1 == sorted2
}
