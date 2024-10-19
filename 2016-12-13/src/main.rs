const START: Pos = Pos {x: 1, y: 1};
const END: Pos = Pos {x: 31, y: 39};

const FAVNUM: usize = 1352;

const COUNT_REACHABLE_AFTER_STEPS: usize = 50;

#[derive(PartialEq)]
struct Pos {
    x: usize,
    y: usize
}


fn main() {
    let mut room = Vec::new();
    let min_steps = move_to(START, &mut room, 0, 9999999, true);
    print_room(&room);
    println!("{min_steps}");
    println!();

    room.clear();
    move_to(START, &mut room, 0, COUNT_REACHABLE_AFTER_STEPS + 1, false);
    let count = count_room(&room);
    println!("{count}")
}


fn move_to(pos: Pos, room: &mut Vec<Vec<i32>>, step: usize, min_steps_so_far: usize, stop_on_end: bool) -> usize {
    while room.len() <= pos.x {
        room.push(Vec::new());
    }
    while room[pos.x].len() <= pos.y {
        let new_y = room[pos.x].len();
        room[pos.x].push(if is_wall(&Pos {x: pos.x, y: new_y}) {-1} else {9999999});
    }

    let val = room[pos.x][pos.y];
    if val < 0 || step >= val.try_into().unwrap() || step >= min_steps_so_far {
        return min_steps_so_far
    }

    room[pos.x][pos.y] = step.try_into().unwrap();

    if stop_on_end && pos == END {
        return step;
    }

    let mut min_steps_so_far = min_steps_so_far;

    for next_move in [(-1, 0), (0, -1), (1, 0), (0, 1)] {
        let next_pos = (pos.x as i32 + next_move.0, pos.y as i32 + next_move.1);
        if next_pos.0 < 0 || next_pos.1 < 0 {
            continue
        }
        let next_pos = Pos {x: next_pos.0 as usize, y: next_pos.1 as usize};
        min_steps_so_far = move_to(next_pos, room, step + 1, min_steps_so_far, stop_on_end);
    }
    min_steps_so_far
}


fn is_wall(pos: &Pos) -> bool {
    if pos == &START || pos == &END {
        false
    } else {
        let mut num = pos.x * pos.x + 3 * pos.x + 2 * pos.x * pos.y + pos.y + pos.y * pos.y + FAVNUM;
        let mut odd = false;
        while num > 0 {
            if num & 1 > 0 {
                odd = !odd;
            }
            num >>= 1;
        }
        odd
    }
}


fn print_room(room: &Vec<Vec<i32>>) {
    for col in room {
        for cell in col {
            print!("{}", if *cell < 0 {"#"} else if *cell > 9999 {"."} else {" "})
        }
        println!("|");
    }
}


fn count_room(room: &Vec<Vec<i32>>) -> i32 {
    let mut count = 0;
    for col in room {
        for cell in col {
            if *cell >= 0 && *cell <= 50 {
                count += 1;
            }
        }
    }
    count
}
