use std::fs;
use std::collections::{HashMap, HashSet};

#[derive(Clone)]
struct Node {
    weight: i32,
    children: HashSet<String>,
    parent: Option<String>
}

fn main() {
    let mut nodes: HashMap<String, Node> = HashMap::new();
    for line in fs::read_to_string("input.txt").unwrap().lines() {
        let mut tokens = line.split_whitespace();
        let name = tokens.next().unwrap();
        let weight_token = tokens.next().unwrap();
        let mut node = Node {
            weight: weight_token[1..weight_token.len() - 1].parse().unwrap(),
            children: HashSet::new(),
            parent: None
        };
        if let Some(_) = tokens.next() {
            for child in tokens {
                node.children.insert(child.replace(",", ""));
            }
        }
        nodes.insert(name.to_string(), node);
    }

    for (parent_name, parent_node) in nodes.clone() {
        for child_node_name in &parent_node.children {
            nodes.get_mut(child_node_name).unwrap().parent = Some(parent_name.clone());
        }
    }

    let mut bottom_name = String::new();
    for (name, node) in &nodes {
        if node.parent == None {
            println!("Bottom program: {name}");
            bottom_name = name.clone();
        }
    }

    // Part 2
    check_program(&nodes, &bottom_name);

}


fn check_program(nodes: &HashMap<String, Node>, program_name: &str) -> i32 {
    let node = &nodes[program_name];
    let mut total_weight = 0;
    let mut child_weights: HashMap<i32, (i32, &str)> = HashMap::new();
    for child in &node.children {
        let weight = check_program(nodes, &child);
        total_weight += weight;
        child_weights.entry(weight).and_modify(|(num, _)| *num += 1).or_insert((1, child));
    }
    if child_weights.len() == 2 {
        let mut target_weight = 0;
        for (child_weight, data) in &child_weights {
            if data.0 > 1 {
                target_weight = *child_weight;
            }
        }
        for (child_weight, data) in child_weights {
            if data.0 == 1 {
                println!("New weight: {}", target_weight - child_weight + nodes[data.1].weight);
                total_weight += target_weight - child_weight;
            }
        }
    } else if child_weights.len() != 1 && !child_weights.is_empty() {
        panic!();
    }
    total_weight + node.weight
}
