use std::fs;

const SIZE: usize = 1000;

fn main() {
    let contents = fs::read_to_string("input.txt").unwrap();
    let mut lights1 = [[false; SIZE]; SIZE];
    let mut lights2 = [[0u32; SIZE]; SIZE];

    for line in contents.lines() {
        let words: Vec<&str> = line.split([' ', ',']).collect();
        let coord_offset;
        let function1: fn(bool) -> bool;
        let function2: fn(u32) -> u32;

        match words[0] {
            "turn" => {
                coord_offset = 2;
                match words[1] {
                    "on"  => {
                        function1 = |_| {true}; 
                        function2 = |x| {x + 1}
                    },
                    "off" => {
                        function1 = |_| {false};
                        function2 = |x| {if x > 0 {x - 1} else {0}}
                    },
                    _ => panic!()
                }
            },
            "toggle" => {
                coord_offset = 1;
                function1 = |x| {!x};
                function2 = |x| {x + 2};
            },
            _ => panic!()
        }
        let (x1, y1, x2, y2) = get_coords(&words, coord_offset);
        for y in y1 ..= y2 {
            for x in x1 ..= x2 {
                lights1[y][x] = function1(lights1[y][x]);
                lights2[y][x] = function2(lights2[y][x]);
            }
        }
    }

    let mut count1 = 0;
    let mut count2 = 0u32;
    for y in 0 ..SIZE {
        for x in 0 .. SIZE {
            if lights1[y][x] {
                count1 += 1
            }
            count2 += lights2[y][x];
        }
    }

    println!("Part one: Number of lights on = {count1}");
    println!("Part two: Total brightness    = {count2}");
}

fn get_coords(words: &Vec<&str>, offset: usize) -> (usize, usize, usize, usize) {
    let x1 = words[offset + 0].parse().unwrap();
    let y1 = words[offset + 1].parse().unwrap();
    let x2 = words[offset + 3].parse().unwrap();
    let y2 = words[offset + 4].parse().unwrap();
    (x1, y1, x2, y2)
}
