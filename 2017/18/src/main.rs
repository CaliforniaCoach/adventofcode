use std::fs;
use std::collections::{HashMap, VecDeque};

fn main() {
    run(false);
    run(true);
}


fn run(send_and_receive_dual_parallel_mode: bool) {
    let mut pc: [i128; 2] = [0; 2];
    let lines: Vec<String> = fs::read_to_string("input.txt").unwrap().trim().lines().map(|x| x.to_string()).collect();
    let mut reg: [HashMap<String, i128>; 2] = [HashMap::new(), HashMap::new()];
    reg[1].insert(String::from("p"), 1);
    let mut last_played = 0;

    // Variables for send/receive dual parallel mode:
    let mut thread = 0;
    let mut queue_to: [VecDeque<i128>; 2] = [const {VecDeque::new()}; 2];
    let mut num_msgs_thread_1_sent = 0;

    while pc[thread] >= 0 && (pc[thread] as usize) < lines.len() {
        let mut reg = &mut reg[thread];
        let mut tok = lines[pc[thread] as usize].split_whitespace();
        let cmd = tok.next().unwrap();
        let arg1 = tok.next().unwrap();
        let arg2 = tok.next();
        match cmd {
            "snd" => {
                if !send_and_receive_dual_parallel_mode {
                    last_played = val(&mut reg, arg1);
                } else {
                    queue_to[1 - thread].push_back(val(&mut reg, arg1));
                    if thread == 1 {
                        num_msgs_thread_1_sent += 1;
                    }
                }
            }

            "set" => {
                let val = val(&mut reg, arg2.unwrap());
                set(&mut reg, arg1, val);
            }

            "add" => {
                let val = val(&mut reg, arg1) + val(&mut reg, arg2.unwrap());
                set(&mut reg, arg1, val);
            }

            "mul" => {
                let val = val(&mut reg, arg1) * val(&mut reg, arg2.unwrap());
                set(&mut reg, arg1, val);
            }

            "mod" => {
                let val = val(&mut reg, arg1) % val(&mut reg, arg2.unwrap());
                set(&mut reg, arg1, val);
            }

            "rcv" => {
                if !send_and_receive_dual_parallel_mode {
                    if val(&mut reg, arg1) != 0 {
                        println!("First recovered sound: {last_played}");
                        break
                    }
                } else {
                    if let Some(val) = queue_to[thread].pop_front() {
                        set(reg, arg1, val);
                    } else {
                        // If queue is empty, switch to other thread
                        thread = 1 - thread;

                        if pc[thread] < 0 || pc[thread] as usize >= lines.len() || queue_to[thread].is_empty() {
                            // If other thread is terminated or queue also empty, terminate all
                            println!("Prg/Thread 1 sent {num_msgs_thread_1_sent} messages");
                            break
                        }

                        // Do not increment program counter, proceed with PC where stopped before
                        continue
                    }
                }
            }

            "jgz" => {
                if val(&mut reg, arg1) > 0 {
                    pc[thread] += val(&mut reg, arg2.unwrap()) - 1;
                }
            }

            _ => {
                panic!("Unknown command '{}'", lines[pc[thread] as usize]);
            }
        }
        pc[thread] += 1;
    }
}


fn val(reg: &mut HashMap<String, i128>, arg: &str) -> i128 {
    if let Ok(val) = arg.parse::<i128>() {
        val
    } else {
        reg.entry(arg.to_string()).or_insert(0);
        reg[arg]
    }
}

fn set(reg: &mut HashMap<String, i128>, reg_name: &str, val: i128) {
    reg.entry(reg_name.to_string()).and_modify(|x| *x = val).or_insert(val);
}
