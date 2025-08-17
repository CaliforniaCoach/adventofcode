use std::fs;
use std::collections::HashMap;

fn main() {
    let mut canvas: Vec<Vec<bool>> = vec!(
        vec!(false, true, false),
        vec!(false, false, true),
        vec!(true, true, true)
    );

    let rules = read_rules("input.txt");

    for _ in 0..5 {
        if canvas.len() % 2 == 0 {
            canvas = mutate(canvas, &rules, 2);
        } else if canvas.len() % 3 == 0 {
            canvas = mutate(canvas, &rules, 3);
        } else {
            panic!("Impossible!");
        }

        print_canvas(&canvas);
        println!();
    }

    println!("Num pixels after 5: {}", count_pixels(&canvas));

    for _ in 5..18 {
        if canvas.len() % 2 == 0 {
            canvas = mutate(canvas, &rules, 2);
        } else if canvas.len() % 3 == 0 {
            canvas = mutate(canvas, &rules, 3);
        } else {
            panic!("Impossible!");
        }
    }

    println!("Num pixels after 18: {}", count_pixels(&canvas));
}


fn read_rules(filename: &str) -> HashMap<Vec<Vec<bool>>, Vec<Vec<bool>>> {
    let mut rules = HashMap::new();

    for line in fs::read_to_string(filename).unwrap().lines() {
        let mut tok1 = line.split(" => ");
        let mut src = Vec::new();
        for tok2 in tok1.next().unwrap().split("/") {
            src.push(tok2.chars().map(|x| match x {'.' => false, '#' => true, _ => panic!("Invalid char")}).collect());
        }
        let mut dest = Vec::new();
        for tok2 in tok1.next().unwrap().split("/") {
            dest.push(tok2.chars().map(|x| match x {'.' => false, '#' => true, _ => panic!("Invalid char")}).collect());
        }
        rules.insert(src, dest);
    }

    rules
}


fn mutate(canvas: Vec<Vec<bool>>, rules: &HashMap<Vec<Vec<bool>>, Vec<Vec<bool>>>, size: usize) -> Vec<Vec<bool>> {
    let mut new_canvas: Vec<Vec<bool>> = Vec::new();

    for square_y in 0..canvas.len() / size {
        for _ in 0..size + 1 {
            new_canvas.push(Vec::new());
        }
        for square_x in 0..canvas.len() / size {
            let mut square: Vec<Vec<bool>> = Vec::new();
            for y in square_y * size..(square_y + 1) * size {
                let mut square_row = Vec::new();
                for x in square_x * size..(square_x + 1) * size {
                    square_row.push(canvas[y][x]);
                }
                square.push(square_row);
            }

            'rotloop: for rot in 0..4 {
                for flip in [false, true] {
                    if let Some(replace) = rules.get(&rot_and_flip(&square, rot, flip)) {
                        for (y, row) in replace.iter().enumerate() {
                            new_canvas[square_y * (size + 1) + y].extend(row);
                        }
                        break 'rotloop
                    }
                }
            }
        }
    }

    new_canvas
}


fn rot_and_flip(square: &Vec<Vec<bool>>, rot: i32, flip: bool) -> Vec<Vec<bool>> {
    let mut result: Vec<Vec<bool>> = Vec::new();
    for y in 0..square.len() {
        let mut row = Vec::new();
        for x in 0..square.len() {
            let x1 = if flip {square.len() - 1 - x} else {x};
            let y1 = y;
            let x2 = match rot {
                0 => x1,
                1 => square.len() - 1 - y1,
                2 => square.len() - 1 - x1,
                3 => y1,
                _ => panic!("Invalid rot")
            };
            let y2 = match rot {
                0 => y1,
                1 => x1,
                2 => square.len() - 1 - y1,
                3 => square.len() - 1 - x1,
                _ => panic!("Invalid rot")
            };
            row.push(square[y2][x2]);
        }
        result.push(row);
    }

    result
}


fn print_canvas(canvas: &Vec<Vec<bool>>) {
    for row in canvas {
        println!("{}", row.iter().map(|x| if *x {'#'} else {'.'}).collect::<String>());
    }
}


fn count_pixels(canvas: &Vec<Vec<bool>>) -> usize {
    canvas.iter().map(|x| x.iter().filter(|x| **x).count()).sum()
}
