use std::{fs, u32};

struct Map {
    map: Vec<Vec<bool>>,
    x_offset: usize,
    y_offset: usize,
    visited: u32
}

impl Map {
    fn test(self: &mut Self, x: i32, y: i32) {
        let mut x_idx: i32 = x + self.x_offset as i32;
        let mut y_idx: i32 = y + self.y_offset as i32;

        if y_idx < 0 {
            self.map.insert(0, vec![false; (x_idx + 1) as usize]);
            self.y_offset += 1;
            y_idx += 1;
        } else if y_idx as usize >= self.map.len() {
            self.map.push(vec![false; (x_idx + 1).try_into().unwrap()]);
        }
        if x_idx < 0 {
            for row in &mut self.map {
                row.insert(0, false);
            }
            self.x_offset += 1;
            x_idx += 1;
        } else {
            while x_idx as usize >= self.map[y_idx as usize].len() {
                self.map[y_idx as usize].push(false);
            }
        }

        if !self.map[y_idx as usize][x_idx as usize] {
            self.visited += 1;
            self.map[y_idx as usize][x_idx as usize] = true;
        }
    }
}


fn new_map() -> Map {
    Map {
        map: vec![vec![true]],
        x_offset: 0,
        y_offset: 0,
        visited: 1
    }
}


fn move_x_y(x: &mut i32, y: &mut i32, ch: char) {
    match ch {
        '^' => *y -= 1,
        'v' => *y += 1,
        '>' => *x += 1,
        '<' => *x -= 1,
        _ => (),
    }
}


fn run_for_n_santas(instructions: &String, n: usize) {
    let mut x = vec![0; n];
    let mut y = vec![0; n];
    let mut map = new_map();

    for (i, ch) in instructions.chars().enumerate() {
        let x = &mut x[i % n];
        let y = &mut y[i % n];
        move_x_y(x, y, ch);
        map.test(*x, *y);
    }

    println!("{}", map.visited)
}


fn main() {
    let content = fs::read_to_string("input.txt").expect("Something went wrong");
    run_for_n_santas(&content, 1);
    run_for_n_santas(&content, 2);
}
