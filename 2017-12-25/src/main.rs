use std::fs;
use std::collections::{HashMap, VecDeque};
use regex::Regex;

#[derive(Debug)]
struct State {
    write_val: (bool, bool),
    move_to: (bool, bool),
    next_state: (String, String)
}

fn main() {
    let first = Regex::new(r"Begin in state (\w+)\.").unwrap();
    let checksum = Regex::new(r"Perform a diagnostic checksum after (\d+) steps\.").unwrap();

    let state_title = Regex::new(r"In state (\w+)\:").unwrap();
    let if_state = Regex::new(r"If the current value is (0|1):").unwrap();
    let write_val = Regex::new(r"- Write the value (0|1)\.").unwrap();
    let move_cmd = Regex::new(r"- Move one slot to the (left|right)\.").unwrap();
    let cont_state = Regex::new(r"- Continue with state (\w+)\.").unwrap();

    let content = fs::read_to_string("input.txt").unwrap();

    let mut states: HashMap<String, State> = HashMap::new();

    let mut it = content.lines().map(|x| x.trim()).filter(|x| x.len() > 0);

    let Some(m) = first.captures(it.next().unwrap()) else {panic!()};
    let mut state = &m[1];

    let Some(m) = checksum.captures(it.next().unwrap()) else {panic!()};
    let num_steps: usize = (&m[1]).parse().unwrap();

    while let Some(line) = it.next() {
        if let Some(m) = state_title.captures(line) {
            let state_name = String::from(&m[1]);
            let mut state = State {write_val: (false, false), move_to: (false, false), next_state: (String::new(), String::new())};

            let Some(m) = if_state.captures(it.next().unwrap()) else {panic!()};
            assert_eq!(&m[1], "0");
            let Some(m) = write_val.captures(it.next().unwrap()) else {panic!()};
            state.write_val.0 = &m[1] == "1";
            let Some(m) = move_cmd.captures(it.next().unwrap()) else {panic!()};
            state.move_to.0 = &m[1] == "right";
            let Some(m) = cont_state.captures(it.next().unwrap()) else {panic!()};
            state.next_state.0 = String::from(&m[1]);

            let Some(m) = if_state.captures(it.next().unwrap()) else {panic!()};
            assert_eq!(&m[1], "1");
            let Some(m) = write_val.captures(it.next().unwrap()) else {panic!()};
            state.write_val.1 = &m[1] == "1";
            let Some(m) = move_cmd.captures(it.next().unwrap()) else {panic!()};
            state.move_to.1 = &m[1] == "right";
            let Some(m) = cont_state.captures(it.next().unwrap()) else {panic!()};
            state.next_state.1 = String::from(&m[1]);

            states.insert(state_name, state);
        } else {
            panic!("Unexpected line: '{line}'");
        }
    }

    let mut tape = Tape::new();

    for _ in 0..num_steps {
        let rule = &states[state];
        let val = tape.get();
        let (write_val, move_to, next_state) = if val {(rule.write_val.1, rule.move_to.1, &rule.next_state.1)} else {(rule.write_val.0, rule.move_to.0, &rule.next_state.0)};
        tape.set(write_val);
        tape.move_cursor(move_to);
        state = next_state;
    }

    println!("Checksum: {}", tape.count_1s());
}


pub struct Tape {
    tape: VecDeque<bool>,
    cursor: isize,
    lowest: isize
}

impl Tape {
    pub fn new() -> Tape {
        let mut tape = Tape {
            tape: VecDeque::new(),
            cursor: 0,
            lowest: 0
        };
        tape.tape.push_back(false);
        tape
    }
    pub fn get(&self) -> bool {
        self.tape[(self.cursor - self.lowest) as usize]
    }

    pub fn set(&mut self, val: bool) {
        self.tape[(self.cursor - self.lowest) as usize] = val;
    }

    pub fn move_cursor(&mut self, left_right: bool) {
        if left_right {
            self.cursor += 1;
            if self.cursor - self.lowest >= self.tape.len() as isize {
                self.tape.push_back(false);
            }
        } else {
            self.cursor -= 1;
            if self.cursor < self.lowest {
                self.tape.push_front(false);
                self.lowest = self.cursor;
            }
        }
    }

    pub fn count_1s(&self) -> usize {
        let mut count = 0;
        for element in &self.tape {
            if *element {
                count += 1;
            }
        }
        count
    }
}
