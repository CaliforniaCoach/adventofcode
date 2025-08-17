use std::fs;

fn main() {
    let content = fs::read_to_string("input.txt").unwrap();
    let mut count1 = 0;
    let mut count2 = 0;
    for line in content.lines() {
        let mut inside_brackets = false;
        let mut is_candidate1 = false;
        let mut is_not_candidate_1 = false;
        let mut is_candidate2 = false;
        let mut aba_pairs: Vec<(char, char)> = Vec::new();
        let mut bab_pairs: Vec<(char, char)> = Vec::new();
        for i in 0..line.len() - 3 {
            let c0 = line.chars().nth(i).unwrap();
            let c1 = line.chars().nth(i + 1).unwrap();
            let c2 = line.chars().nth(i + 2).unwrap();
            let c3 = line.chars().nth(i + 3).unwrap();
            if c0 == '[' {
                inside_brackets = true;
            } else if c0 == ']' {
                inside_brackets = false;
            }
            if c0 == c3 && c1 == c2 && c0 != c1 {
                if inside_brackets {
                    is_not_candidate_1 = true;
                } else {
                    is_candidate1 = true;
                }
            }
            if !is_candidate2 && c0 == c2 && c0 != c1 {
                if inside_brackets {
                    if aba_pairs.contains(&(c1, c0)) {
                        is_candidate2 = true;
                        continue;
                    }
                    bab_pairs.push((c0, c1));
                } else {
                    if bab_pairs.contains(&(c1, c0)) {
                        is_candidate2 = true;
                        continue;
                    }
                    aba_pairs.push((c0, c1));
                }
            }
        }
        let c_1 = line.chars().nth_back(0).unwrap();
        let c_2 = line.chars().nth_back(1).unwrap();
        let c_3 = line.chars().nth_back(2).unwrap();
        if !is_candidate2 && c_1 == c_3 && c_1 != c_2 && bab_pairs.contains(&(c_2, c_1)) {
            is_candidate2 = true;
        }
        if is_candidate1 && !is_not_candidate_1 {
            count1 += 1;
        }
        if is_candidate2 {
            count2 += 1;
        }
    }
    println!("{count1}");
    println!("{count2}");
}
