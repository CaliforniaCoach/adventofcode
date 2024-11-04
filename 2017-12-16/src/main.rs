use std::fs;
use std::collections::{VecDeque, HashMap};

fn main() {
    let mut pos: VecDeque<u32> = VecDeque::new();
    let mut states: HashMap<VecDeque<u32>, u32> = HashMap::new();
    for prg in 0..16 {
        pos.push_back(prg);
    }
    dance(&mut pos);
    for prg in &pos {
        print!("{}", ('a' as u8 + *prg as u8) as char);
    }
    println!();

    states.insert(pos.clone(), 0);

    let mut i = 1;
    let mut search_for_cycle = true;
    while i < 1_000_000_000 {
        dance(&mut pos);
        if search_for_cycle {
            if let Some(last_i) = states.get(&pos) {
                let cycle = i - last_i;
                i += ((1_000_000_000 - i) / cycle) * cycle;
                search_for_cycle = false;
            } else {
                states.insert(pos.clone(), i);
            }
        }
        i += 1;
    }

    for prg in &pos {
        print!("{}", ('a' as u8 + *prg as u8) as char);
    }
    println!();
}


fn dance(pos: &mut VecDeque<u32>) {
    for line in fs::read_to_string("input.txt").unwrap().trim().split(",") {
        match &line[0..1] {
            "s" => for _ in 0..line[1..].parse().unwrap() {
                let prg = pos.pop_back().unwrap();
                pos.push_front(prg);
            },
            "x" => {
                let mut tok  = line[1..].split("/");
                let pos1 = tok.next().unwrap().parse().unwrap();
                let pos2 = tok.next().unwrap().parse().unwrap();
                pos.swap(pos1, pos2);
            },
            "p" => {
                let prg1= line.bytes().nth(1).unwrap() - 'a' as u8;
                let prg2= line.bytes().nth(3).unwrap() - 'a' as u8;
                let mut pos1 = 0;
                let mut pos2 = 0;
                for i in 0..pos.len() {
                    if pos[i] == prg1 as u32 {
                        pos1 = i;
                    } else if pos[i] == prg2 as u32 {
                        pos2 = i;
                    }
                }
                pos.swap(pos1, pos2);
            }
            _ => {
                panic!("Unknown move '{line}");
            }
        }
    }
}
