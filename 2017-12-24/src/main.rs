use std::fs;

fn main() {
    let mut components: Vec<(i32, i32)> = Vec::new();
    for line in fs::read_to_string("input.txt").unwrap().lines() {
        let mut tok = line.split("/");
        components.push((tok.next().unwrap().parse().unwrap(), tok.next().unwrap().parse().unwrap()));
    }

    let mut max_strength = 0;
    let mut max_longest_strength = 0;
    let mut max_length = 0;
    find_next(&mut components, 0, 0, 0, &mut max_strength, &mut max_longest_strength, &mut max_length);
    println!("Maximum strength altogether: {max_strength}");
    println!("Maximum strength of longest: {max_longest_strength}");
}


fn find_next(components: &mut Vec<(i32, i32)>, port_pins: i32, current_strength: i32, length: i32, max_strength_so_far: &mut i32, max_longest_strength_so_far: &mut i32, max_length: &mut i32) {
    if length > *max_length {
        *max_length = length;
        *max_longest_strength_so_far = current_strength;
    } else if length == *max_length {
        *max_longest_strength_so_far = current_strength.max(*max_longest_strength_so_far);
    }
    for i in 0..components.len() {
        let component = components[i];
        for (pins0, pins1) in [(component.0, component.1), (component.1, component.0)] {
            if pins0 == port_pins {
                let new_current_strength = current_strength + pins0 + pins1;
                *max_strength_so_far = new_current_strength.max(*max_strength_so_far);
                components[i] = (-1, -1);
                find_next(components, pins1, new_current_strength, length + 1, max_strength_so_far, max_longest_strength_so_far, max_length);
                components[i] = component;
                break
            }
        }
    }
}
