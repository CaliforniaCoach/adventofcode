use std::fs;
use std::collections::HashMap;
use itertools::Itertools;

#[derive(Debug, PartialEq)]
enum Tile {
    Wall,
    Open,
    Number(u32)
}

fn main() {
    let mut maze: Vec<Vec<Tile>> = Vec::new();
    let mut numbers: HashMap<u32, (usize, usize)> = HashMap::new();

    for (y, line) in fs::read_to_string("input.txt").unwrap().lines().enumerate() {
        maze.push(Vec::new());
        for (x, chr) in line.chars().enumerate() {
            maze.last_mut().unwrap().push(match chr {
                '#' => Tile::Wall,
                '.' => Tile::Open,
                _ => {
                    let num = chr.to_digit(10).unwrap();
                    numbers.entry(num).and_modify(|_| panic!("Double number!")).or_insert((x, y));
                    Tile::Number(num)
                }
            })
        }
    }

    let mut distances: HashMap<(u32, u32), u32> = HashMap::new();
    for combi in numbers.keys().sorted().combinations(2) {
        let from = (numbers[combi[0]].0, numbers[combi[0]].1);
        let to = (numbers[combi[1]].0, numbers[combi[1]].1);
        distances.insert((*combi[0], *combi[1]), distance(&maze, from, to));
    }

    println!("Shortest path length without return to 0: {}", calc_shortest_path(&numbers.keys().collect(), &distances, false));
    println!("Shortest path length with return to 0: {}", calc_shortest_path(&numbers.keys().collect(), &distances, true));
}


fn distance(maze: &Vec<Vec<Tile>>, from: (usize, usize), to: (usize, usize)) -> u32 {
    let mut maze2: Vec<Vec<u32>> = Vec::new();
    for row in maze {
        maze2.push(Vec::new());
        for _tile in row {
            maze2.last_mut().unwrap().push(u32::MAX);
        }
    }

    next_step(maze, &mut maze2, from.0 as i32, from.1 as i32, to.0 as i32, to.1 as i32, 0, u32::MAX)
}


fn next_step(maze: &Vec<Vec<Tile>>, maze2: &mut Vec<Vec<u32>>, x: i32, y: i32, to_x: i32, to_y: i32, step: u32, min_so_far: u32) -> u32 {
    let mut min_so_far = min_so_far;
    if x == to_x && y == to_y {
        return if step < min_so_far {step} else {min_so_far};
    }
    if step >= min_so_far || step >= maze2[y as usize][x as usize] || maze[y as usize][x as usize] == Tile::Wall {
        return min_so_far;
    }

    maze2[y as usize][x as usize] = step;

    for (dx, dy) in [(1, 0), (0, 1), (-1, 0), (0, -1)] as [(i32, i32); 4] {
        min_so_far = next_step(maze, maze2, x + dx, y + dy, to_x, to_y, step + 1, min_so_far);
    }

    min_so_far
}


fn calc_shortest_path(numbers: &Vec<&u32>, distances: &HashMap<(u32, u32), u32>, return_to_0: bool) -> u32 {
    let mut shortest_path = u32::MAX;
    for path in numbers.iter().filter(|&&x| *x != 0).permutations(numbers.len() - 1) {
        let mut path_len = *distances.get(&(0, **path[0])).unwrap();
        for i in 1..path.len() {
            let num1 = *path[i - 1];
            let num2 = *path[i];
            let num_from = if num1 > num2 {num2} else {num1};
            let num_to = if num1 > num2 {num1} else {num2};
            path_len += *distances.get(&(*num_from, *num_to)).unwrap();
        }
        if return_to_0 {
            path_len += distances.get(&(0, ***path.last().unwrap())).unwrap();
        }
        if path_len < shortest_path {
            shortest_path = path_len;
        }
    }
    shortest_path
}
