use std::fs;
use std::array;
use std::cmp::{min, max};
use regex::Regex;

const CHIP_LOW: usize = 17;
const CHIP_HIGH: usize = 61;

#[derive(Copy, Clone)]
enum Rule {
    Bot(usize),
    Output(usize),
    Nothing
}

fn main() {
    let content = fs::read_to_string("input.txt").unwrap();

    let mut bot_chips: [Vec<usize>; 1000] = array::from_fn(|_| Vec::new());
    let mut out_chips: [Vec<usize>; 1000] = array::from_fn(|_| Vec::new());
    let mut bot_rules = [[Rule::Nothing; 2]; 1000];

    let value = Regex::new(r"value (\d+) goes to bot (\d+)").unwrap();
    let bot = Regex::new(r"bot (\d+) gives low to (bot|output) (\d+) and high to (bot|output) (\d+)").unwrap();
    
    for line in content.lines() {
        if let Some(m) = value.captures(line) {
            let bot_no: usize = m[2].parse().unwrap();
            let chip_no: usize = m[1].parse().unwrap();
            bot_chips[bot_no].push(chip_no);
        } else if let Some(m) = bot.captures(line) {
            let bot_no: usize = m[1].parse().unwrap();
            if &m[2] == "bot" {
                bot_rules[bot_no][0] = Rule::Bot(m[3].parse().unwrap());
            } else {
                bot_rules[bot_no][0] = Rule::Output(m[3].parse().unwrap());
            }
            if &m[4] == "bot" {
                bot_rules[bot_no][1] = Rule::Bot(m[5].parse().unwrap());
            } else {
                bot_rules[bot_no][1] = Rule::Output(m[5].parse().unwrap());
            }
        } else {
            panic!("Unknwon rule {line}");
        }
    }

    let mut bot_holds_two = true;
    while bot_holds_two {
        bot_holds_two = false;
        for i in 0..bot_chips.len() {
            let chips = &bot_chips[i];
            let rule = &bot_rules[i];
            if chips.len() == 2 {
                bot_holds_two = true;
                let chip1 = *chips.get(0).unwrap();
                let chip2 = *chips.get(1).unwrap();
                let low = min(chip1, chip2);
                let high = max(chip1, chip2);
                match rule[0] {
                    Rule::Bot(i) => bot_chips[i].push(low),
                    Rule::Output(i) => out_chips[i].push(low),
                    _ => panic!("Something went wrong")
                }
                match rule[1] {
                    Rule::Bot(k) => bot_chips[k].push(high),
                    Rule::Output(k) => out_chips[k].push(high),
                    _ => panic!("Something went wrong")
                }
                bot_chips[i].clear();
                if low == CHIP_LOW && high == CHIP_HIGH {
                    println!("Bot {i} compares {CHIP_LOW} and {CHIP_HIGH}")
                }
            } else if chips.len() > 2 {
                panic!("Bot {i} contains too many chips");
            }
        }
    }
    let mut mul = 1;
    for i in 0..3 {
        if out_chips[i].len() != 1 {
            panic!("Output {i} has number of chips not equal to 1");
        }
        print!("Output {i}: ");
        for chip in &out_chips[i] {
            print!("{chip}, ");
            mul *= chip;
        }
        println!();
    }
    println!("Product: {mul}");
}
