use std::fs;
use regex::Regex;

fn main() {
    solve_for("abcde", "test.txt", false);
    solve_for("abcdefgh", "input.txt", false);

    solve_for("fbgdceah", "input.txt", true);
}

#[derive(Clone)]
enum ReversePos {
    Unique(usize),
    None,
    Ambiguous
}

fn solve_for(start_password: &str, filename: &str, reverse: bool) {
    let re_swap_pos = Regex::new(r"swap position (\d+) with position (\d+)").unwrap();
    let re_swap_letter = Regex::new(r"swap letter (\w) with letter (\w)").unwrap();
    let re_rot_left = Regex::new(r"rotate left (\d+) steps?").unwrap();
    let re_rot_right = Regex::new(r"rotate right (\d+) steps?").unwrap();
    let re_rot_pos = Regex::new(r"rotate based on position of letter (\w)").unwrap();
    let re_reverse = Regex::new(r"reverse positions (\d+) through (\d+)").unwrap();
    let re_move = Regex::new(r"move position (\d+) to position (\d+)").unwrap();

    let mut pw = start_password.to_string();

    let mut lines: Vec<String> = fs::read_to_string(filename).unwrap().lines().map(|l| l.to_string()).collect();
    if reverse {
        lines.reverse();
    }

    let mut reverse_map: Vec<ReversePos> = vec!(ReversePos::None; start_password.len());
    if reverse {
        for old_pos in 0..start_password.len() {
            let new_pos = (old_pos * 2 + if old_pos >= 4 {2} else {1}) % start_password.len();
            reverse_map[new_pos] = match reverse_map[new_pos] {
                ReversePos::None => ReversePos::Unique(old_pos),
                ReversePos::Unique(_) => ReversePos::Ambiguous,
                ReversePos::Ambiguous => ReversePos::Ambiguous
            };
        }
    }

    for line in lines {
        let line = &line;
        if let Some(m) = re_swap_pos.captures(line) {
            let pos1 = m[1].parse::<usize>().unwrap();
            let pos2 = m[2].parse::<usize>().unwrap();
            let swap = pw.chars().nth(pos1).unwrap();
            pw.replace_range(pos1..=pos1, &pw.chars().nth(pos2).unwrap().to_string());
            pw.replace_range(pos2..=pos2, &swap.to_string());
        } else if let Some(m) = re_swap_letter.captures(line) {
            let ch1 = m[1].chars().nth(0).unwrap();
            let ch2 = m[2].chars().nth(0).unwrap();
            pw = pw.chars().map(|c| {if c == ch1 {ch2} else if c == ch2 {ch1} else {c}}).collect();
        } else if let Some(m) = re_rot_left.captures(line) {
            let num = m[1].parse::<usize>().unwrap();
            if reverse {
                pw = rot_right(pw, num);
            } else {
                pw = pw[num..].to_string() + &pw[..num];
            }
        } else if let Some(m) = re_rot_right.captures(line) {
            let num = m[1].parse::<usize>().unwrap();
            if reverse {
                pw = pw[num..].to_string() + &pw[..num];
            } else {
                pw = rot_right(pw, num);
            }
        } else if let Some(m) = re_rot_pos.captures(line) {
            let pos = pw.find(&m[1]).unwrap();
            if reverse {
                if let ReversePos::Unique(orig_pos) = reverse_map[pos] {
                    pw = rot_right(pw, (orig_pos + start_password.len() - pos) % start_password.len());
                } else {
                    panic!("Impossible to reverse scrambling");
                }
            } else {
                pw = rot_right(pw, pos + if pos >= 4 {2} else {1});
            }
        } else if let Some(m) = re_reverse.captures(line) {
            let from = m[1].parse::<usize>().unwrap();
            let to = m[2].parse::<usize>().unwrap();
            pw = pw[..from].to_string() + &pw[from..=to].chars().rev().collect::<String>() + &pw[to + 1..];
        } else if let Some(m) = re_move.captures(line) {
            let mut pos1 = m[1].parse::<usize>().unwrap();
            let mut pos2 = m[2].parse::<usize>().unwrap();
            if reverse {
                let swap = pos1;
                pos1 = pos2;
                pos2 = swap;
            }
            let ch = pw.remove(pos1);
            pw.insert(pos2, ch);
        } else {
            panic!("Unknown command: '{line}'");
        }
    }
    if reverse {
        println!("Reverse-scrambled password of {start_password} is {pw}");
    } else {
        println!("Forward-scrambled password of {start_password} is {pw}");
    }
}


fn rot_right(str: String, num: usize) -> String {
    let pos = str.len() - num % str.len();
    str[pos..].to_string() + &str[..pos]
}
