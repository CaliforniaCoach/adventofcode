use std::fs;
use regex::Regex;
use std::collections::HashMap;

fn main() {
    let re = Regex::new(r"^(\w+) (inc|dec) (-?\d+) if (\w+) (>|<|>=|<=|==|!=) (-?\d+)$").unwrap();
    let mut reg: HashMap<String, i128> = HashMap::new();
    let mut max_total = 0;
    for line in fs::read_to_string("input.txt").unwrap().lines() {
        if let Some(m) = re.captures(line) {
            let regname1 = m[1].to_string();
            let regname2 = m[4].to_string();
            reg.entry(regname1.clone()).or_insert(0);
            reg.entry(regname2.clone()).or_insert(0);
            let reg2val = reg[&regname2];
            let compval: i128 = m[6].parse().unwrap();
            let condition = match &m[5] {
                ">" => reg2val > compval,
                "<" => reg2val < compval,
                ">=" => reg2val >= compval,
                "<=" => reg2val <= compval,
                "==" => reg2val == compval,
                "!=" => reg2val != compval,
                _ => panic!("Unknown comparison operator '{}'", &m[5])
            };
            if condition {
                let incval = &m[3].parse::<i128>().unwrap() * match &m[2] {
                    "inc" => 1,
                    "dec" => -1,
                    _ => panic!("Unknown inc/dec statement '{}'", &m[2])
                };
                let newval = reg[&regname1] + incval;
                reg.entry(regname1).and_modify(|x| *x = newval);
                if newval > max_total {
                    max_total = newval;
                }
            }
        } else {
            panic!("Unknown command: '{line}'");
        }
    }

    let mut max = None;
    for val in reg.values() {
        match max {
            None => max = Some(val),
            Some(x) => if val > x {max = Some(val)}
        }
    }

    println!("Max val at end:         {}", max.unwrap());
    println!("Max val during process: {}", max_total);
}
