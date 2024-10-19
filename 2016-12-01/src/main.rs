use std::fs;
use regex::Regex;


struct Delta {
    x: i32,
    y: i32
}

#[derive(PartialEq, Clone, Copy)]
struct Point {
    x: i32,
    y: i32
}

const DIRECTION_DELTA: [Delta; 4] = [
    Delta {x: 0, y: 1},
    Delta {x: 1, y: 0},
    Delta {x: 0, y: -1},
    Delta {x: -1, y: 0}
];

fn main() {
    let contents = fs::read_to_string("input.txt").unwrap();

    let mut direction = 0;
    let mut point = Point {x: 0, y: 0};

    let mut visited: Vec<Point> = Vec::new();
    visited.push(point);
    let mut point_visited_twice = Point {x: 0, y: 0};
    let mut point_visited_twice_found = false;

    for step in Regex::new(r"(?m)[,\s]+").unwrap().split(contents.trim()) {
        match step.chars().nth(0).unwrap() {
            'R' => direction = (direction + 1) % 4,
            'L' => direction = (direction + 4 - 1) % 4,
            _ => panic!("Unexpected direction")
        }
        let dist = step[1..].parse::<i32>().unwrap();
        for _ in 0..dist {
            point.x += DIRECTION_DELTA[direction].x;
            point.y += DIRECTION_DELTA[direction].y;
            if !point_visited_twice_found {
                if visited.contains(&point) {
                    point_visited_twice = point;
                    point_visited_twice_found = true;
                } else {
                    visited.push(point);
                }
            }
        }
    }

    let taxi_dist1 = point.x.abs() + point.y.abs();
    let taxi_dist2 = point_visited_twice.x.abs() + point_visited_twice.y.abs();
    println!("{taxi_dist1}");
    println!("{taxi_dist2}");
}
