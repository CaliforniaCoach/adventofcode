use std::fs;
use regex::Regex;

fn main() {

    let mut reg = [0; 4];
    run(&mut reg);
    println!("a = {}", reg[0]);

    let mut reg = [0, 0, 1, 0];
    run(&mut reg);
    println!("a = {}", reg[0]);
}

fn run(reg: &mut [i32; 4]) {
    let cpy = Regex::new(r"cpy (\w+) ([a-d])").unwrap();
    let inc = Regex::new(r"inc ([a-d])").unwrap();
    let dec = Regex::new(r"dec ([a-d])").unwrap();
    let jnz = Regex::new(r"jnz (\w+) (-?\d+)").unwrap();

    let mut pc: i32 = 0;

    let content = fs::read_to_string("input.txt").unwrap();
    let lines: Vec<&str> = content.lines().collect();

    loop {
        let line = lines[pc as usize];
        if let Some(m) = cpy.captures(line) {
            reg[reg_to_idx(&m[2])] = val(&m[1], &reg);
        } else if let Some(m) = inc.captures(line) {
            reg[reg_to_idx(&m[1])] += 1
        } else if let Some(m) = dec.captures(line) {
            reg[reg_to_idx(&m[1])] -= 1
        } else if let Some(m) = jnz.captures(line) {
            if val(&m[1], &reg) != 0 {
                pc += m[2].parse::<i32>().unwrap() - 1;
            }
        } else {
            panic!("Unknown command: {line}");
        }
        pc += 1;
        if pc < 0 {
            panic!("Jump to PC below 0");
        } else if pc as usize >= lines.len() {
            break
        }
    }
}

fn reg_to_idx(reg: &str) -> usize {
    (reg.bytes().nth(0).unwrap() - b'a') as usize
}

fn val(arg: &str, reg: &[i32; 4]) -> i32 {
    match arg.parse::<i32>() {
        Ok(num) => num,
        Err(_) => reg[reg_to_idx(arg)]
    }
}
