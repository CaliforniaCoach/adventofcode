use std::fs;

fn main() {
    let mut sum = 0;

    for line in fs::read_to_string("input.txt").unwrap().lines() {
        let mut min = i32::MAX;
        let mut max = i32::MIN;

        for num in line.split_whitespace() {
            let val: i32 = num.parse().unwrap();
            if val < min {
                min = val;
            }
            if val > max {
                max = val;
            }
        }

        sum += max - min;
    }

    println!("{sum}");

    // Part 2

    sum = 0;
    let mut numbers: Vec<i32> = Vec::new();
    for line in fs::read_to_string("input.txt").unwrap().lines() {
        numbers.clear();

        for num in line.split_whitespace() {
            let val1: i32 = num.parse().unwrap();
            for val2 in &numbers {
                if val1 % val2 == 0 {
                    sum += val1 / val2;
                    break;
                } else if val2 % val1 == 0 {
                    sum += val2 / val1;
                    break;
                }
            }
            numbers.push(val1);
        }
    }
    println!("{sum}");
}
