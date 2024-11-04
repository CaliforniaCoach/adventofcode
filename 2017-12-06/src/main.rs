use std::fs;
use std::collections::HashMap;

fn main() {
    let mut banks: Vec<u32> = fs::read_to_string("input.txt").unwrap().split_whitespace().map(|x| x.parse().unwrap()).collect();
    // banks = vec!(0, 2, 7, 0);
    let len = banks.len();
    let mut states: HashMap<Vec<u32>, u32> = HashMap::new();
    let mut count = 0;
    while !states.contains_key(&banks) {
        states.insert(banks.clone(), count);
        let mut max = 0;
        let mut idx = 0;
        for (i, bank) in banks.iter().enumerate() {
            if *bank > max {
                max = *bank;
                idx = i;
            }
        }
        banks[idx] = 0;
        while max > 0 {
            idx += 1;
            banks[idx % len] += 1;
            max -= 1;
        }
        count += 1;
    }
    println!("Cycles: {count}, loop length: {}", count - states[&banks]);
}
