use std::fs;

fn main() {
    let mut p: Vec<(i32, i32, i32)> = Vec::new();
    let mut v: Vec<(i32, i32, i32)> = Vec::new();
    let mut a: Vec<(i32, i32, i32)> = Vec::new();

    for line in fs::read_to_string("input.txt").unwrap().lines() {
        let mut line = line.to_string();
        for del in ["p=<", "v=<", "a=<", ">", " "] {
            line = line.replace(del, "");
        }
        let mut tok = line.split(',');
        p.push((tok.next().unwrap().parse().unwrap(), tok.next().unwrap().parse().unwrap(), tok.next().unwrap().parse().unwrap()));
        v.push((tok.next().unwrap().parse().unwrap(), tok.next().unwrap().parse().unwrap(), tok.next().unwrap().parse().unwrap()));
        a.push((tok.next().unwrap().parse().unwrap(), tok.next().unwrap().parse().unwrap(), tok.next().unwrap().parse().unwrap()));
    }

    let mut min_acceleration = i32::MAX;
    let mut min_particle = 0;
    for (particle, acc) in a.iter().enumerate() {
        let acceleration = acc.0.abs() + acc.1.abs() + acc.2.abs();
        if acceleration < min_acceleration {
            min_acceleration = acceleration;
            min_particle = particle;
        }
    }

    println!("Particle with minimum acceleration: {min_particle}");

    let mut t = 0;
    loop {
        let mut i = 0;
        while i < p.len() {
            let mut collision = false;
            let pos1 = p[i];
            let mut k = i + 1;
            while k < p.len() {
                if p[k] == pos1 {
                    collision = true;
                    p.remove(k);
                    v.remove(k);
                    a.remove(k);
                } else {
                    k += 1;
                }
            }
            if collision {
                p.remove(i);
                v.remove(i);
                a.remove(i);
            } else {
                i += 1;
            }
        }

        for i in 0..p.len() {
            v[i].0 += a[i].0;
            v[i].1 += a[i].1;
            v[i].2 += a[i].2;
            p[i].0 += v[i].0;
            p[i].1 += v[i].1;
            p[i].2 += v[i].2;
        }
        println!("Particles left: {}", p.len());
    }
}
