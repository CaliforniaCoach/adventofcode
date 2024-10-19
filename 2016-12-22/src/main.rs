use std::fs;
use regex::Regex;

#[derive(Clone)]
struct Node {
    x: usize,
    y: usize,
    size: usize,
    used: usize,
    avail: usize,
    use_percent: usize
}

fn main() {
    let re = Regex::new(r"/dev/grid/node-x(\d+)-y(\d+)\s+(\d+)T\s+(\d+)T\s+(\d+)T\s+(\d+)%").unwrap();
    let ignore: Vec<Regex> = vec!(
        Regex::new(r"root@ebhq-gridcenter# df -h").unwrap(),
        Regex::new(r"Filesystem\s+Size\s+Used\s+Avail\s+Use%").unwrap()
    );

    let mut nodes: Vec<Vec<Option<Node>>> = Vec::new();

    'lines: for line in fs::read_to_string("input.txt").unwrap().lines() {
        if let Some(m) = re.captures(line) {
            let node = Node {
                x: m[1].parse().unwrap(),
                y: m[2].parse().unwrap(),
                size: m[3].parse().unwrap(),
                used: m[4].parse().unwrap(),
                avail: m[5].parse().unwrap(),
                use_percent: m[6].parse().unwrap()
            };
            let x = node.x;
            let y = node.y;
            while nodes.len() <= x {
                nodes.push(Vec::new());
            }
            while nodes[x].len() <= y {
                nodes[x].push(None);
            }
            nodes[x][y] = Some(node);
        } else {
            for ignore_pattern in &ignore {
                if ignore_pattern.is_match(line) {
                    continue 'lines
                }
            }
            panic!("Unnkown line: '{line}'")
        }
    }

    let mut num_viable = 0;
    for node_row in &nodes {
        for node in node_row {
            if let Some(node) = node {
                if node.used != 0 {
                    for node_row2 in &nodes {
                        for node2 in node_row2 {
                            if let Some(node2) = node2 {
                                if (node.x != node2.x || node.y != node2.y) && node.used <= node2.avail {
                                    num_viable += 1;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    println!("Part 1: Num viable pairs: {num_viable}");

    println!();
    println!("Part 2: Map:");
    let mut empty_x = 0;
    let mut empty_y = 0;
    for y in 0..nodes[0].len() {
        for x in 0..nodes.len() {
            print!("{}", match nodes[x][y].clone().unwrap().used {
                0 => {empty_x = x; empty_y = y; '_'},
                1..101 => '.',
                _ => '#'
            });
        }
        println!();
    }
    println!("Empty node at x: {}, y: {}; grid width x height: {} x {}",
        empty_x, empty_y, nodes.len(), nodes[0].len()
    );

    println!("{}", 3 + 20 + (35 - 1 - 1) * (1 + 5) + 1);
}
