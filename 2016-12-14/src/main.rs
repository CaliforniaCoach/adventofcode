use md5;
use fancy_regex::Regex;
use std::collections::VecDeque;

fn main() {
    solve_for_stretches("abc", 1);
    solve_for_stretches("abc", 2017);
    solve_for_stretches("zpqevtbw", 1);
    solve_for_stretches("zpqevtbw", 2017);
}

fn solve_for_stretches(salt: &str, num_hashings: usize) {
    let three_consecutive = Regex::new(r"(.)\1\1").unwrap();
    let five_consecutive = Regex::new(r"(.)\1\1\1\1").unwrap();
    let mut num_valid_keys = 0;
    let mut postfix_number = 0;
    let mut candidates: VecDeque<(i32, String)> = VecDeque::new();
    'main: loop {
        let key = salt.to_owned() + &postfix_number.to_string();
        let mut digest = key;
        for _ in 0..num_hashings {
            digest = format!("{:x}", md5::compute(digest));
        }
        if five_consecutive.is_match(&digest).unwrap() {
            while !candidates.is_empty() && candidates.front().unwrap().0 < postfix_number - 1000 {
                candidates.pop_front();
            }
            let mut i = 0;
            while i < candidates.len() {
                let key_index = candidates[i].0;
                let repeated_digit = &candidates[i].1;
                if digest.contains(&repeated_digit.repeat(5)) {
                    num_valid_keys += 1;
                    if num_valid_keys >= 64 {
                        println!("{key_index}");
                        break 'main
                    }
                    candidates.remove(i);
                } else {
                    i += 1;
                }
            }
        }
        if let Ok(Some(m)) = three_consecutive.captures(&digest) {
            candidates.push_back((postfix_number, m[1].to_string()));
        }
        postfix_number += 1;
    }
}
 