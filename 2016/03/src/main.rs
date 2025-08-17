use std::fs;

fn valid_triangle(a: i32, b:i32, c:i32) -> i32 {
    if a + b > c && a + c > b && b + c > a {
        return 1;
    } else {
        return 0;
    }
}

fn main() {
    let content = fs::read_to_string("input.txt").unwrap();
    let mut count = 0;
    for line in content.lines() {
        let tokens: Vec<&str> = line.split_whitespace().collect();
        let a = tokens[0].parse::<i32>().unwrap();
        let b = tokens[1].parse::<i32>().unwrap();
        let c = tokens[2].parse::<i32>().unwrap();
        count += valid_triangle(a, b, c);
    }
    println!("{count}");

    // Part 2
    count = 0;
    let mut line_num = 0;
    let mut a = [0, 0, 0];
    let mut b = [0, 0, 0];
    let mut c = [0, 0, 0];
    for line in content.lines() {
        let tokens: Vec<&str> = line.split_whitespace().collect();
        a[line_num] = tokens[0].parse::<i32>().unwrap();
        b[line_num] = tokens[1].parse::<i32>().unwrap();
        c[line_num] = tokens[2].parse::<i32>().unwrap();
        line_num += 1;
        if line_num >= 3 {
            count += valid_triangle(a[0], a[1], a[2]);
            count += valid_triangle(b[0], b[1], b[2]);
            count += valid_triangle(c[0], c[1], c[2]);
            line_num = 0;
        }
    }
    println!("{count}");
}
