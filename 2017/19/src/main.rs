use std::fs;

fn main() {
    let content = fs::read_to_string("input.txt").unwrap();
    let field: Vec<&str> = content.lines().collect();

    let mut x = 0;
    let mut y = 0;
    let (mut dx, mut dy) = (0i32, 1i32);

    let mut visited: Vec<char> = Vec::new();
    let mut steps = 1;

    for chr in field[0].chars() {
        if chr == '|' {
            break
        }
        x += 1;
    }

    loop {
        if !contains_pipe(&field, x + dx, y + dy) {
            (dx, dy) = (dy.abs(), dx.abs());
            if !contains_pipe(&field, x + dx, y + dy) {
                (dx, dy) = (-dx, -dy);
                if !contains_pipe(&field, x + dx, y + dy) {
                    break
                }
            }
        }
        (x, y) = (x + dx, y + dy);
        let chr = field[y as usize].chars().nth(x as usize).unwrap();
        if chr >= 'A' && chr <= 'Z' {
            visited.push(chr);
        }
        steps += 1;
    }

    println!("Letters visited in order: {}", String::from_iter(visited.iter()));
    println!("Number of total steps: {steps}");
}


fn contains_pipe(field: &Vec<&str>, x: i32, y: i32) -> bool {
    !(x < 0 || y < 0 || y as usize >= field.len() || x as usize >= field[y as usize].len() || field[y as usize].chars().nth(x as usize).unwrap() == ' ')
}
