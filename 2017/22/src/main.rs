use std::fs;

#[derive(Clone, PartialEq)]
enum State {
    Clean,
    Weakened,
    Infected,
    Flagged
}

fn main() {
    let mut nodes: Vec<Vec<bool>> = fs::read_to_string("input.txt").unwrap().lines().map(|x| x.chars().map(|x| match x {'.' => false, '#' => true, _ => panic!("Invalid char")}).collect()).collect();

    let (mut x, mut y) = (nodes[0].len() / 2, nodes.len() / 2);
    let (mut dx, mut dy) = (0i32, -1i32);

    let mut count = 0;

    for _ in 0..10000 {
        if nodes[y][x] {
            (dx, dy) = (-dy, dx);
        } else {
            (dx, dy) = (dy, -dx);
        }

        if y == 0 && dy < 0 {
            nodes.insert(0, vec!(false; nodes[0].len()));
            y += 1;
        } else if y >= nodes.len() - 1 && dy > 0 {
            nodes.push(vec!(false; nodes[0].len()));
        }
        if x == 0 && dx < 0 {
            for row in &mut nodes {
                row.insert(0, false);
                x += 1;
            }
        } else if x >= nodes[0].len() - 1 && dx > 0 {
            for row in &mut nodes {
                row.push(false);
            }
        }

        nodes[y][x] = !nodes[y][x];
        if nodes[y][x] {
            count += 1;
        }
        (x, y) = ((x as i32 + dx) as usize, (y as i32 + dy) as usize);
    }

    println!("{count} infected");


    // Part 2

    let mut nodes: Vec<Vec<State>> = fs::read_to_string("input.txt").unwrap().lines().map(|x| x.chars().map(|x| match x {'.' => State::Clean, '#' => State::Infected, _ => panic!("Invalid char")}).collect()).collect();

    let (mut x, mut y) = (nodes[0].len() / 2, nodes.len() / 2);
    let (mut dx, mut dy) = (0i32, -1i32);

    let mut count = 0;

    for _ in 0..10000000 {
        (dx, dy, nodes[y][x]) = match nodes[y][x] {
            State::Clean => (dy, -dx, State::Weakened),
            State::Weakened => (dx, dy, State::Infected),
            State::Infected => (-dy, dx, State::Flagged),
            State::Flagged => (-dx, -dy, State::Clean)
        };

        if y == 0 && dy < 0 {
            nodes.insert(0, vec!(State::Clean; nodes[0].len()));
            y += 1;
        } else if y >= nodes.len() - 1 && dy > 0 {
            nodes.push(vec!(State::Clean; nodes[0].len()));
        }
        if x == 0 && dx < 0 {
            for row in &mut nodes {
                row.insert(0, State::Clean);
            }
            x += 1;
        } else if x >= nodes[0].len() - 1 && dx > 0 {
            for row in &mut nodes {
                row.push(State::Clean);
            }
        }

        if nodes[y][x] == State::Infected {
            count += 1;
        }
        (x, y) = ((x as i32 + dx) as usize, (y as i32 + dy) as usize);
    }

    println!("{count} infected");
}
