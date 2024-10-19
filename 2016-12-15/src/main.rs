use std::fs;
use regex::Regex;
use num::integer::lcm;

const FILENAME: &str = "input.txt";


struct Disc {
    disc_no: i64,
    num_pos: i64,
    pos_0: i64
}

fn main() {
    let mut discs: Vec<Disc> = Vec::new();
    let regex = Regex::new(r"Disc #(\d+) has (\d+) positions; at time=0, it is at position (\d+)\.").unwrap();
    for line in fs::read_to_string(FILENAME).unwrap().lines() {
        let captures = regex.captures(line).unwrap();
        discs.push(Disc {
            disc_no: captures[1].parse().unwrap(),
            num_pos: captures[2].parse().unwrap(),
            pos_0: captures[3].parse().unwrap()
        });
    }
    solve(&discs);

    discs.push(Disc{
        disc_no: discs.last().unwrap().disc_no + 1,
        num_pos: 11,
        pos_0: 0
    });
    solve(&discs);
}


fn solve(discs: &Vec<Disc>) {

    // Solve with Chinese remainder theorem

    // Convert disc data to parameters of Chinese remainder theorem
    let mut m: Vec<i64> = Vec::new();
    let mut a: Vec<i64> = Vec::new();
    for disc in discs {
        let mi = disc.num_pos;
        let ai = disc.num_pos - disc.pos_0 - disc.disc_no;
        m.push(mi);
        a.push(ai);
    }

    // Calculate least common multiple of <m>s
    #[allow(non_snake_case)]
    let mut M = 1;
    for mi in &m {
        M = lcm(M, *mi);
    }

    // Calculate <e>s with extended Euclidean algorithm, use result for solution <x>
    let mut x = 0;
    for (ai, mi) in a.into_iter().zip(m) {
        #[allow(non_snake_case)]
        let Mi = M / mi;
        let (gcd, _ri, si) = egcd(mi as i64, Mi as i64);
        // Ensure that <m> and <M> are prime to each other
        if gcd != 1 {
            panic!("Not solvable with this algorithm, sorry.")
        }
        let ei = si * M / mi;
        x += ai * ei;
    }

    let smallest_x = (x % M + M) % M;
    println!("LCM: {M}, One solution: {x}, Smallest positive solution: {smallest_x}");
}


/// Returns `(g, s, t)` such that `g == gcd(a, b)` and `a*s + t*b == g`.
fn egcd(mut a: i64, mut b: i64) -> (i64, i64, i64) {
    let (mut sa, mut ta, mut sb, mut tb) = (1, 0, 0, 1);
    while b != 0 {
        let (q, r) = (a / b, a % b);
        (sa, ta, sb, tb) = (sb, tb, sa - q * sb, ta - q * tb);
        (a, b) = (b, r);
    }
    (a, sa, ta)
}
