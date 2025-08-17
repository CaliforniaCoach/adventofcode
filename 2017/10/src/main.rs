use std::fs;

const SIZE: usize = 256;
const FILE: &str = "input.txt";

fn main() {
    let mut list = [0; SIZE];
    for i in 1..list.len() {
        list[i] = i;
    }
    let mut cur: usize = 0;
    let mut skipsize = 0;
    for token in fs::read_to_string(FILE).unwrap().trim().split(",") {
        let length: usize = token.parse().unwrap();
        for i in 0..(length / 2) as usize {
            let swap = list[(cur + i) % list.len()];
            list[(cur + i) % list.len()] = list[(cur + (length - i - 1)) % list.len()];
            list[(cur + (length - i - 1)) % list.len()] = swap;
        }
        cur = (cur + length + skipsize) % list.len();
        skipsize += 1;
    }

    println!("{}", list[0] * list[1]);


    // Part 2

    let mut list = [0; SIZE];
    for i in 1..list.len() {
        list[i] = i;
    }
    let mut cur: usize = 0;
    let mut skipsize = 0;
    let mut seq: Vec<u8> = fs::read_to_string(FILE).unwrap().trim().bytes().collect();
    for x in [17, 31, 73, 47, 23] {
        seq.push(x);
    }

    for _ in 0..64 {
        for length in &seq {
            for i in 0..(length / 2) as usize {
                let swap = list[(cur + i) % list.len()];
                list[(cur + i) % list.len()] = list[(cur + (*length as usize - i - 1)) % list.len()];
                list[(cur + (*length as usize - i - 1)) % list.len()] = swap;
            }
            cur = (cur + *length as usize + skipsize) % list.len();
            skipsize += 1;
        }
    }

    for group in 0..16 {
        let mut val = list[group * 16];
        for i in 1..16 {
            val ^= list[group * 16 + i];
        }
        print!("{:02x}", val);
    }
    println!();
}
