use std::fs;
use regex::Regex;
use std::sync::LazyLock;
use std::collections::{HashMap, HashSet};

fn main() {
    let mut i = 1i128;
    loop {
        let mut reg = [i, 0, 0, 0];
        println!("Trying with {i}...");
        if run(&mut reg) {
            break
        }
        i += 1;
    }
    println!("Solution: a = {i}");
}


static CPY: LazyLock<Regex> = LazyLock::new(|| Regex::new(r"cpy (-?\w+) ([a-d])").unwrap());
static INC: LazyLock<Regex> = LazyLock::new(|| Regex::new(r"inc ([a-d])").unwrap());
static DEC: LazyLock<Regex> = LazyLock::new(|| Regex::new(r"dec ([a-d])").unwrap());
static JNZ: LazyLock<Regex> = LazyLock::new(|| Regex::new(r"jnz (\w+) (-?\w+)").unwrap());
static OUT: LazyLock<Regex> = LazyLock::new(|| Regex::new(r"out (\w+)").unwrap());


#[derive(PartialEq)]
enum Arg {
    Reg(usize),
    Val(i128)
}

impl From<&str> for Arg {
    fn from(text: &str) -> Self {
        match text.parse::<i128>() {
            Ok(num) => Arg::Val(num),
            Err(_) => Arg::Reg(reg_to_idx(text))
        }
    }
}


struct Mul {
    multiplier: Arg,
    multiplier_sign: i8,
    multiplicand_reg: usize,
    multiplicand_sign: i8,
    product_reg: usize,
    product_sign: i8
}


fn run(reg: &mut [i128; 4]) -> bool {

    let mut pc: i128 = 0;

    let content = fs::read_to_string("input.txt").unwrap();
    let lines: Vec<String> = content.lines().map(|x| x.to_string()).collect();

    let mult_patterns = identify_mult_patterns(&lines);

    let mut last_out: Option<i128> = None;
    let mut states: HashSet<[i128; 4]> = HashSet::new();

    loop {
        let line = &lines[pc as usize];
        if let Some(mul) = mult_patterns.get(&(pc as usize)) {
            let multiplier_val = mul.multiplier_sign as i128 * match mul.multiplier {
                Arg::Reg(reg_index) => reg[reg_index],
                Arg::Val(val) => val
            };
            let multiplicand_val = mul.multiplicand_sign as i128 * reg[mul.multiplicand_reg];
            if multiplier_val <= 0 || multiplicand_val <= 0 {
                panic!("Impossible multiplication operation");
            }
            reg[mul.product_reg] += mul.product_sign as i128 * multiplier_val * multiplicand_val;
            pc += 5;
        }
        else if let Some(m) = CPY.captures(&line) {
            reg[reg_to_idx(&m[2])] = val(&m[1], &reg);
        } else if let Some(m) = INC.captures(&line) {
            reg[reg_to_idx(&m[1])] += 1
        } else if let Some(m) = DEC.captures(&line) {
            reg[reg_to_idx(&m[1])] -= 1
        } else if let Some(m) = JNZ.captures(&line) {
            if val(&m[1], &reg) != 0 {
                pc += val(&m[2], &reg) - 1;
            }
        } else if let Some(m) = OUT.captures(&line) {
            let out_val = val(&m[1], reg);
            if out_val == 0 {
                if last_out != None && last_out != Some(1) {
                    return false
                } else {
                    if states.contains(reg) {
                        return true
                    } else {
                        states.insert(*reg);
                    }
                }
            } else if out_val == 1 {
                if last_out != Some(0) {
                    return false
                }
            } else {
                return false
            }
            last_out = Some(out_val);
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
    false
}


fn reg_to_idx(reg: &str) -> usize {
    (reg.bytes().nth(0).unwrap() - b'a') as usize
}


fn val(arg: &str, reg: &[i128; 4]) -> i128 {
    match arg.parse::<i128>() {
        Ok(num) => num,
        Err(_) => reg[reg_to_idx(arg)]
    }
}


fn identify_mult_patterns(lines: &Vec<String>) -> HashMap<usize, Mul> {
    let mut mult_map = HashMap::new();
    for i in 0..lines.len() - 5 {
        // First line must be "cpy" instruction
        if let Some(m) = CPY.captures(&lines[i]) {
            let mut mul = Mul {
                multiplier: Arg::Reg(0),
                multiplier_sign: 0,
                multiplicand_reg: 0,
                multiplicand_sign: 0,
                product_reg: 0,
                product_sign:0
            };
            mul.multiplier = Arg::from(&m[1]);
            let helper_reg1 = reg_to_idx(&m[2]);

            // Seond line must be "inc" or "dec" instruction
            if let Some(m) = INC.captures(&lines[i + 1]) {
                mul.product_reg = reg_to_idx(&m[1]);
                mul.product_sign = 1;
            } else if let Some(m) = DEC.captures(&lines[i + 1]) {
                mul.product_reg = reg_to_idx(&m[1]);
                mul.product_sign = -1;
            } else {
                continue
            }
            if mul.product_reg == helper_reg1 || Arg::Reg(mul.product_reg) == mul.multiplier {
                continue
            }

            // Third line must be "inc" or "dec" instruction, helper register same as in first line
            if let Some(m) = INC.captures(&lines[i + 2]) {
                if reg_to_idx(&m[1]) != helper_reg1 {
                    continue
                }
                mul.multiplier_sign = -1;
            } else if let Some(m) = DEC.captures(&lines[i + 2]) {
                if reg_to_idx(&m[1]) != helper_reg1 {
                    continue
                }
                mul.multiplier_sign = 1;
            } else {
                continue
            }

            // Fourth line must be "jnz <reg> -2" instruction, helper register same as in first line
            if let Some(m) = JNZ.captures(&lines[i + 3]) {
                if reg_to_idx(&m[1]) != helper_reg1 || Arg::from(&m[2]) != Arg::Val(-2) {
                    continue
                }
            } else {
                continue
            }

            // Fifth line must be "inc" or "dec" instruction, register different to helper register and mulitplier
            if let Some(m) = INC.captures(&lines[i + 4]) {
                mul.multiplicand_reg = reg_to_idx(&m[1]);
                mul.multiplicand_sign = -1;
            } else if let Some(m) = DEC.captures(&lines[i + 4]) {
                mul.multiplicand_reg = reg_to_idx(&m[1]);
                mul.multiplicand_sign = 1;
            } else {
                continue
            }
            if mul.multiplicand_reg == helper_reg1 || mul.multiplicand_reg == mul.product_reg || Arg::Reg(mul.multiplicand_reg) == mul.multiplier {
                continue
            }

            // Sixth line must be "jnz <reg> -5" instruction, register same as multplier register
            if let Some(m) = JNZ.captures(&lines[i + 5]) {
                if reg_to_idx(&m[1]) != mul.multiplicand_reg || Arg::from(&m[2]) != Arg::Val(-5) {
                    continue
                }
            } else {
                continue
            }

            // If we reach this point, all conditions are met --> add entry
            mult_map.insert(i, mul);
        }
    }

    mult_map
}
