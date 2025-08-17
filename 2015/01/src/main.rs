use std::fs;

fn main() {
    let content = fs::read_to_string("input.txt").expect("Something went wrong");

    let mut floor = 0;
    let mut basement_reached = false;
    let mut basement_reached_after = 0;
    for ch in content.chars() {
        if ch == '(' {
            floor = floor + 1;
        } else if ch == ')' {
            floor = floor - 1;
        }
        if !basement_reached {
            basement_reached_after = basement_reached_after + 1;
            if floor == -1 {
                basement_reached = true;
            }
        }
    }

    println!("Final Floor: {floor}");
    println!("Basement reached after {basement_reached_after} moves");
}
