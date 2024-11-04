const STEPS: usize = 380;

fn main() {
    let (cur, buf) = spin(2017);
    println!("Value after current position: {}", buf[(cur + 1) % buf.len()]);

    let after_zero = spin2(50_000_000);
    println!("Value after zero: {after_zero}");
}


fn spin(spins: u32) -> (usize, Vec<u32>) {
    let mut buf: Vec<u32> = vec!(0);
    let mut cur = 0;

    for i in 0..spins {
        cur = (cur + STEPS + 1) % buf.len();
        buf.insert(cur, i + 1);
    }

    (cur, buf)
}


fn spin2(spins: u32) -> u32 {
    let mut cur = 0;
    let mut buflen = 1;
    let mut after_zero = 0;

    for i in 0..spins {
        cur = (cur + STEPS) % buflen  + 1;
        if cur == 1 {
            after_zero = i + 1;
        }
        buflen += 1;
    }

    after_zero
}
