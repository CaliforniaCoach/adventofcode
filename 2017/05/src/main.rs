use std::fs;

fn main() {
    let mut prg: Vec<i32> = fs::read_to_string("input.txt").unwrap().lines().map(|x| x.parse().unwrap()).collect();
    let mut pc_new: i32 = 0;
    let mut count = 0;
    while pc_new >= 0 && pc_new < prg.len() as i32 {
        let pc_cur = pc_new as usize;
        pc_new += prg[pc_cur];
        prg[pc_cur] += 1;
        count += 1;
    }
    println!("{count}");

    let mut prg: Vec<i32> = fs::read_to_string("input.txt").unwrap().lines().map(|x| x.parse().unwrap()).collect();
    let mut pc_new: i32 = 0;
    let mut count = 0;
    while pc_new >= 0 && pc_new < prg.len() as i32 {
        let pc_cur = pc_new as usize;
        pc_new += prg[pc_cur];
        prg[pc_cur] += if prg[pc_cur] < 3 {1} else {-1};
        count += 1;
    }
    println!("{count}");
}
