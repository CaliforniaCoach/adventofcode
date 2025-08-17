use std::fs;
use regex::Regex;


const WIDTH: usize = 50;
const HEIGHT: usize = 6;

fn main() {
    let mut pixel = [[false; HEIGHT]; WIDTH];

    let regex_rect = Regex::new(r"rect (\d+)x(\d+)").unwrap();
    let regex_rotate_row = Regex::new(r"rotate row y=(\d+) by (\d+)").unwrap();
    let regex_rotate_col = Regex::new(r"rotate column x=(\d+) by (\d+)").unwrap();

    let content = fs::read_to_string("input.txt").unwrap();

    for line in content.lines() {
        if let Some(cmd) = regex_rect.captures(line) {
            let x: usize = (&cmd[1]).parse().unwrap();
            let y: usize = (&cmd[2]).parse().unwrap();
            for ix in 0..x {
                for iy in 0..y {
                    pixel[ix][iy] = true;
                }
            }
        } else if let Some(cmd) = regex_rotate_row.captures(line) {
            let row: usize = (&cmd[1]).parse().unwrap();
            let num: usize = (&cmd[2]).parse().unwrap();
            let mut temp = [false; WIDTH];
            for i in 0..WIDTH {
                temp[(i + num) % WIDTH] = pixel[i][row];
            }
            for i in 0..WIDTH {
                pixel[i][row] = temp[i];
            }
        } else if let Some(cmd) = regex_rotate_col.captures(line) {
            let col: usize = (&cmd[1]).parse().unwrap();
            let num: usize = (&cmd[2]).parse().unwrap();
            let mut temp = [false; HEIGHT];
            for i in 0..HEIGHT {
                temp[(i + num) % HEIGHT] = pixel[col][i];
            }
            for i in 0..HEIGHT {
                pixel[col][i] = temp[i];
            }
        } else {
            panic!("Unknown command {line}");
        }
    }

    let mut count = 0;
    for y in 0..HEIGHT {
        if y % HEIGHT == 0 {
            println!();
        }
        for x in 0..WIDTH {
            if x % 5 == 0 {
                print!("   ");
            }
            if pixel[x][y] {
                print!("*");
                count += 1;
            } else {
                print!(" ");
            }
        }
        println!();
}
    println!("Count: {count}");
}
