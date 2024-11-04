use std::fs;

fn main() {
    let mut inside_garbage = false;
    let mut ignore_next = false;
    let mut group_level = 0;

    let mut total_score = 0;
    let mut garbage_chars = 0;

    for chr in fs::read_to_string("input.txt").unwrap().trim().chars() {
        if ignore_next {
            ignore_next = false;
        } else if inside_garbage {
            if chr == '>' {
                inside_garbage = false;
            } else if chr == '!' {
                ignore_next = true;
            } else {
                garbage_chars += 1;
            }
        } else if chr == '<' {
            inside_garbage = true;
        } else if chr == '{' {
            group_level += 1;
            total_score += group_level;
        } else if chr == '}' {
            group_level -= 1;
            if group_level < 0 {
                panic!();
            }
        } else if chr != ',' {
            panic!("Unexpected character '{}'", chr);
        }
    }
    println!("Score:              {total_score}");
    println!("Garbage characters: {garbage_chars}");
}
