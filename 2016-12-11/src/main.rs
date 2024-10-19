use std::collections::{HashSet, HashMap};
use itertools::Itertools;

const MAX_OBJECTS: usize = 2;
const MIN_OBJECTS: usize = 1;

const NUM_FLOORS: usize = 4;

#[derive(Eq, PartialEq, Hash, Clone)]
enum Object {
    Generator(usize),
    Microchip(usize)
}

fn main() {
    /*
    // Test data
    let initial_floors: [HashSet<Object>; NUM_FLOORS] = [
        HashSet::from([Object::Microchip(0), Object::Microchip(1)]),
        HashSet::from([Object::Generator(0)]),
        HashSet::from([Object::Generator(1)]),
        HashSet::from([])
    ];
    */

    // 0: polonium
    // 1: thulium
    // 2: promethium
    // 3: ruthenium
    // 4: cobalt
    let mut initial_floors: [HashSet<Object>; NUM_FLOORS] = [
        HashSet::from([Object::Generator(0), Object::Generator(1), Object::Microchip(1), Object::Generator(2), Object::Generator(3), Object::Microchip(3), Object::Generator(4), Object::Microchip(4)]),
        HashSet::from([Object::Microchip(0), Object::Microchip(2)]),
        HashSet::from([]),
        HashSet::from([])
    ];

    let mut already_visited: HashMap<([(usize, usize); NUM_FLOORS], usize), usize> = HashMap::new();
    let min_steps = try_next(&initial_floors, &mut already_visited, 0, 0, 9999999);
    println!("Solution part 1: {min_steps}");
    println!();

    initial_floors[0].insert(Object::Generator(5));
    initial_floors[0].insert(Object::Generator(6));
    initial_floors[0].insert(Object::Microchip(5));
    initial_floors[0].insert(Object::Microchip(6));

    already_visited.clear();
    let min_steps = try_next(&initial_floors, &mut already_visited, 0, 0, 9999999);
    println!("Solution part 2: {min_steps}");
}

fn try_next(floors: &[HashSet<Object>; NUM_FLOORS], already_visited: &mut HashMap<([(usize, usize); NUM_FLOORS], usize), usize>, floor: usize, steps_so_far: usize, min_so_far: usize) -> usize {

    // println!("Moves: {steps_so_far}:");
    // print_floors(&floors);

    if steps_so_far >= min_so_far {
        return min_so_far
    }
    let mut min_so_far = min_so_far;

    let mut solution_found = true;
    for i in 0..NUM_FLOORS - 1 {
        if !floors[i].is_empty() {
            solution_found = false;
            break
        }
    }
    if solution_found {
        println!("Solution found: {steps_so_far} steps");
        return steps_so_far
    }

    let mut floors_pattern = [(0usize, 0usize); NUM_FLOORS];
    for i in 0..NUM_FLOORS {
        let mut num_generators = 0;
        let mut num_microchips = 0;
        for object in &floors[i] {
            match object {
                Object::Generator(_) => num_generators += 1,
                Object::Microchip(_) => num_microchips += 1
            }
            floors_pattern[i] = (num_generators, num_microchips);
        }
    }
    if let Some(min_moves) = already_visited.get_mut(&(floors_pattern, floor)) {
        if steps_so_far >= *min_moves {
            return min_so_far
        } else {
            *min_moves = steps_so_far;
        }
    } else {
        already_visited.insert((floors_pattern, floor), steps_so_far);
    }

    for delta_floor in [1, -1] {
        let target_floor = floor as i32 + delta_floor;
        if target_floor < 0 || target_floor >= NUM_FLOORS as i32 {
            continue
        }
        if delta_floor < 0 {
            let mut all_empty = true;
            for test_floor in 0..floor {
                if !floors[test_floor].is_empty() {
                    all_empty = false;
                    break
                }
            }
            if all_empty {
                continue
            }
        }
        let target_floor = target_floor as usize;
        for num_objects in MIN_OBJECTS..MAX_OBJECTS + 1 {
            'combis: for combi in floors[floor].iter().cloned().combinations(num_objects) {
                let objects_to_move = HashSet::from_iter(combi);
                let new_target_floor = &floors[target_floor as usize] | &objects_to_move;
                for object in &new_target_floor {
                    // Do test for all microchips
                    if let Object::Microchip(chip) = object {
                        if (&new_target_floor).contains(&Object::Generator(*chip)) {
                            // If matching generator present, this microchip is safe
                            continue
                        }
                        for other_object in &new_target_floor {
                            if let Object::Generator(generator) = other_object {
                                if chip != generator {
                                    // If generator is present not matching microchip: no good
                                    continue 'combis
                                }
                            }
                        }
                    }
                }
                // All clear, no fried microchips with this combination
                let mut new_floors = floors.clone();
                new_floors[target_floor] = new_target_floor;
                new_floors[floor] = &floors[floor] ^ &objects_to_move;
                min_so_far = try_next(&new_floors, already_visited, target_floor, steps_so_far + 1, min_so_far);
            }
        }
    }
    min_so_far
}


/*
fn print_floors(floors: &[HashSet<Object>; NUM_FLOORS]) {
    for floor in (0..NUM_FLOORS).rev() {
        let floor1 = floor + 1;
        print!("Floor {floor1}: ");
        for object in &floors[floor] {
            let obj = match object {
                Object::Generator(i) => format!("G{i}"),
                Object::Microchip(i) => format!("M{i}")
            };
            print!("{obj} ");
        }
        println!();
    }
    println!();
}
*/
