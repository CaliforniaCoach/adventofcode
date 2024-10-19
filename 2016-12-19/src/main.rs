use std::time::SystemTime;

const NUM_ELVES: usize = 3001330;

// 1808357 is too high

fn main() {
    println!("Part 1 quick solution");
    solve_quick();

    println!();
    println!("Part 2 quick solution");
    solve_part2_quick_for(5);
    solve_part2_quick_for(NUM_ELVES);

    println!();
    println!("Part 2 slow solution:");
    solve_part2_for(6);
    solve_part2_for(NUM_ELVES);

    println!();
    println!("Part 1 with slow solution:");
    solve_for(NUM_ELVES);

    /*
    for i in 1..NUM_ELVES {
        solve_for(i);
    }
    */
}


fn solve_for(num_elves: usize) {
    let start = SystemTime::now();
    let mut next_report = 10;

    let mut elve_presents = Vec::new();
    for i in 0..num_elves {
        elve_presents.push((i, 1));
    }

    let mut i = 0;
    while elve_presents.len() > 1 {
        let i_next = (i + 1) % elve_presents.len();
        elve_presents[i] = (elve_presents[i].0, elve_presents[i].1 + elve_presents[i_next].1);
        elve_presents.remove(i_next);
        i = if i < elve_presents.len() {(i + 1) % elve_presents.len()} else {0};

        let elapsed = start.elapsed().unwrap().as_secs();
        if elapsed >= next_report {
            println!("    Remaining elves: {}", elve_presents.len());
            next_report = elapsed + 10;
        }
    }
    println!("Part 1 solution: {num_elves} elves: Elve {} gets all presents.", elve_presents.first().unwrap().0 + 1);
}

fn solve_part2_for(num_elves: usize) {
    let start = SystemTime::now();
    let mut next_report = 10;

    let mut elve_presents = Vec::new();
    for i in 0..num_elves {
        elve_presents.push(i);
    }

    let mut i = 0;
    while elve_presents.len() > 1 {
        let i_steal_from = (i + elve_presents.len() / 2) % elve_presents.len();
        elve_presents.remove(i_steal_from);
        if i_steal_from < i {
            i -= 1;
        }
        i = if i < elve_presents.len() {(i + 1) % elve_presents.len()} else {0};

        let elapsed = start.elapsed().unwrap().as_secs();
        if elapsed >= next_report {
            println!("    Remaining elves: {}", elve_presents.len());
            next_report = elapsed + 10;
        }
    }
    println!("Part 2 solution: {num_elves} elves: Elve {} gets all presents.", elve_presents.first().unwrap() + 1);
}


fn solve_quick() {
    let mut elve_presents = vec!(1; NUM_ELVES);
    let mut elve = 0;
    loop {
        if elve_presents[elve] > 0 {
            let mut next_elve = (elve + 1) % NUM_ELVES;
            while elve_presents[next_elve] == 0 {
                next_elve = (next_elve + 1) % NUM_ELVES;
            }
            if next_elve == elve {
                println!("Part 1 solution: {NUM_ELVES} elves: Elve {} gets all presents.", elve + 1);
                break
            }
            elve_presents[elve] += elve_presents[next_elve];
            elve_presents[next_elve] = 0;
        }
        elve = (elve + 1) % NUM_ELVES;
    }
}


fn solve_part2_quick_for(num_elves: usize) {
    let mut elve_still_there = vec!(true; num_elves);
    let mut elves_turn = 0;
    let mut elve_to_steal_from = num_elves / 2;
    let mut num_elves_left = num_elves;
    while num_elves_left > 1 {
        elve_still_there[elve_to_steal_from] = false;

        num_elves_left -= 1;

        // Proceed to next elve who's turn it is
        elves_turn = (elves_turn + 1) % num_elves;
        while !elve_still_there[elves_turn] {
            elves_turn = (elves_turn + 1) % num_elves;
        }

        // Proceed to next elve to steal from who is still there
        elve_to_steal_from = (elve_to_steal_from + 1) % num_elves;
        while !elve_still_there[elve_to_steal_from] {
            elve_to_steal_from = (elve_to_steal_from + 1) % num_elves;
        }

        // If num_elves_left was even before and odd now, num_elves_left / 2 is now one less than
        // before. Therefore, we do not need to proceed to next elve to steal from. In the opposite
        // case, we do have to proceed by one.
        if num_elves_left % 2 == 0 {
            elve_to_steal_from = (elve_to_steal_from + 1) % num_elves;
            while !elve_still_there[elve_to_steal_from] {
                elve_to_steal_from = (elve_to_steal_from + 1) % num_elves;
            }
        }
    }
    println!("Part 2 solution: {num_elves} elves: Elve {} gets all presents.", elves_turn + 1);
}
