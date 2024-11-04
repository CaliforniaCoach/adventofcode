use std::fs;

fn main() {
    let mut layer: Vec<(i32, i32)> = Vec::new();
    for line in fs::read_to_string("input.txt").unwrap().lines() {
        let mut tokens = line.split_whitespace();
        let layer_no: i32 = tokens.next().unwrap().replace(":", "").parse().unwrap();
        let depth: i32 = tokens.next().unwrap().parse().unwrap();
        layer.push((layer_no, depth));
    }

    let mut severity = 0;
    for (layer_no, depth) in &layer {
        if layer_no % ((depth - 1) * 2) == 0 {
            severity += layer_no * depth;
        }
    }

    println!("Severity: {severity}");


    // Part 2

    let mut delay = 0;
    'main_loop: loop {
        for (layer_no, depth) in &layer {
            if (layer_no + delay) % ((depth - 1) * 2) == 0 {
                delay += 1;
                continue 'main_loop;
            }
        }
        break;
    }

    println!("Delay: {delay}");
}
