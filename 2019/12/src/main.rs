use std::fs;
use std::collections::HashMap;
use num::integer;
use regex::Regex;

fn main() {
    println!("Tests:");
    let positions = load_positions("test1.txt");
    println!("Should be 179:");
    run(&positions, 10);
    println!("Should be 2772:");
    find_loop(&positions);

    println!("\nInput:");
    let positions = load_positions("input.txt");
    run(&positions, 1000);
    find_loop(&positions);
}

fn load_positions(filename: &str) -> Vec<Vec<i64>> {
    let re = Regex::new(r"[-\d]+").unwrap();
    let content = fs::read_to_string(filename).unwrap();
    let lines = content.lines();
    lines.map(|x| re.find_iter(x).map(|x| x.as_str().parse::<i64>().unwrap()).collect()).collect()
}

fn step(positions: &mut Vec<Vec<i64>>, velocities: &mut Vec<Vec<i64>>) {
    for ipos1 in 0..positions.len() {
        for ipos2 in 0..positions.len() {
            if ipos1 != ipos2 {
                for icoord in 0..3 {
                    if positions[ipos2][icoord] > positions[ipos1][icoord] {
                        velocities[ipos1][icoord] += 1;
                    } else if positions[ipos2][icoord] < positions[ipos1][icoord] {
                        velocities[ipos1][icoord] -= 1;
                    }
                }
            }
        }
    }
    for ipos in 0..positions.len() {
        for icoord in 0..3 {
            positions[ipos][icoord] += velocities[ipos][icoord]
        }
    }
}

fn run(positions: &Vec<Vec<i64>>, num_steps: i32) {
    let mut positions = positions.clone();
    let mut velocities = vec![vec![0; 3]; positions.len()];

    for _ in 0..num_steps {
        step(&mut positions, &mut velocities);
    }

    let mut total_energy: i64 = 0;
    for i in 0..positions.len() {
        let mut potential_energy: i64 = 0;
        let mut kinetic_energy: i64 = 0;
        for coord in 0..3 {
            potential_energy += positions[i][coord].abs();
            kinetic_energy += velocities[i][coord].abs();
        }
        total_energy += potential_energy * kinetic_energy
    }

    println!("{}", total_energy)
}

fn find_loop(positions: &Vec<Vec<i64>>) {
    let mut positions = positions.clone();
    let mut velocities = vec![vec![0; 3]; positions.len()];

    let mut states: [HashMap<Vec<i64>, u64>; 3] = [HashMap::new(), HashMap::new(), HashMap::new()];
    let mut cycles: [u64; 3] = [0; 3];
    let mut offsets: [u64; 3] = [0; 3];

    let mut i = 0;
    'main: loop {
        for coord in 0..3 {
            if cycles[coord] == 0 {
                let mut state: Vec<i64> = Vec::new();
                for pos in &positions {
                    state.push(pos[coord]);
                }
                for vel in &velocities {
                    state.push(vel[coord]);
                }
                if states[coord].contains_key(&state) {
                    offsets[coord] = *states[coord].get(&state).unwrap();
                    cycles[coord] = i - *states[coord].get(&state).unwrap();
                    if cycles.iter().all(|x| *x > 0) {
                        break 'main;
                    }
                }
                states[coord].insert(state, i);
            }
        }
        step(&mut positions, &mut velocities);
        i += 1;
    }
    if offsets.iter().all(|&x| x == 0) {
        println!("{}", integer::lcm(cycles[0], integer::lcm(cycles[1], cycles[2])));
    } else {
        println!("This program needs some more sophisticated evaluation.")
    }
}
