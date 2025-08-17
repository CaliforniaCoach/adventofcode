use std::fs;

fn main() {
    let mut programs: Vec<Vec<usize>> = Vec::new();

    for line in fs::read_to_string("input.txt").unwrap().lines() {
        let mut it = line.split_whitespace();
        if it.next().unwrap().parse::<usize>().unwrap() != programs.len() {
            panic!("Unexpected Line '{line}'");
        }
        programs.push(Vec::new());
        it.next();
        for prg in it {
            programs.last_mut().unwrap().push(prg.replace(",", "").parse().unwrap());
        }
    }

    let mut connected_to_zero = vec!(false; programs.len());
    check_program(&programs, &mut connected_to_zero, 0);

    let mut count = 0;
    for program in &connected_to_zero {
        if *program {
            count += 1;
        }
    }

    println!("Programs in zero group: {count}");


    // Part 2

    let mut group_count = 1;
    for i in 1..connected_to_zero.len() {
        if !connected_to_zero[i] {
            check_program(&programs, &mut connected_to_zero, i);
            group_count += 1;
        }
    }

    println!("Number of groups: {group_count}");
}


fn check_program(program: &Vec<Vec<usize>>, connected_to_zero: &mut Vec<bool>, program_no: usize) {
    if !connected_to_zero[program_no] {
        connected_to_zero[program_no] = true;
        for next_program in &program[program_no] {
            check_program(program, connected_to_zero, *next_program);
        }
    }
}
