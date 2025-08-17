use std::fs;


const MOVE: [[i32; 4]; 13] = [
    [0x1, 0x3, 0x1, 0x1], // 1
    [0x2, 0x6, 0x3, 0x2], // 2
    [0x1, 0x7, 0x4, 0x2], // 3
    [0x4, 0x8, 0x4, 0x3], // 4
    [0x5, 0x5, 0x6, 0x5], // 5
    [0x2, 0xA, 0x7, 0x5], // 6
    [0x3, 0xB, 0x8, 0x6], // 7
    [0x4, 0xC, 0x9, 0x7], // 8
    [0x9, 0x9, 0x9, 0x8], // 9
    [0x6, 0xA, 0xB, 0xA], // A
    [0x7, 0xD, 0xC, 0xA], // B
    [0x8, 0xC, 0xC, 0xB], // C
    [0xB, 0xD, 0xD, 0xD], // D
];


fn main() {
    let content = fs::read_to_string("input.txt").unwrap();

    let mut pos: i32 = 5;
    let mut code = String::from("");

    for line in content.lines() {
        for char in line.chars() {
            match char {
                'U' => if pos - 3 >= 1 {pos -= 3},
                'D' => if pos + 3 <= 9 {pos += 3},
                'R' => if pos % 3 != 0 {pos += 1},
                'L' => if pos % 3 != 1 {pos -= 1},
                _ => panic!("Unexpected letter")
            }
        }
        code.push(char::from_digit(pos as u32, 10).unwrap());
    }
    println!("{code}");

    // Part 2
    pos = 5;
    code.clear();

    for line in content.lines() {
        for char in line.chars() {
            let i: usize;
            match char {
                'U' => i = 0,
                'D' => i = 1,
                'R' => i = 2,
                'L' => i = 3,
                _ => panic!("Unexpected letter")
            }
            pos = MOVE[(pos - 1) as usize][i];
        }
        code.push(char::from_digit(pos as u32, 14).unwrap());
    }
    println!("{code}");
}
