const START_VAL_A: u64 = 783;
const START_VAL_B: u64 = 325;

const MUL_A: u64 = 16807;
const MUL_B: u64 = 48271;

const DIVISOR: u64 = 2147483647;

const NUM_STEPS_PART_1: usize = 40_000_000;
const NUM_STEPS_PART_2: usize = 5_000_000;

const MUST_BE_MULTIPLE_OF_A: u64 = 4;
const MUST_BE_MULTIPLE_OF_B: u64 = 8;

fn main() {
    let mut a: u64 = START_VAL_A;
    let mut b: u64 = START_VAL_B;

    let mut count = 0;
    for _ in 0..NUM_STEPS_PART_1 {
        a = next_val(a, MUL_A, 1);
        b = next_val(b, MUL_B, 1);

        if a & 0b1111111111111111 == b & 0b1111111111111111 {
            count += 1;
        }
    }
    println!("{count}");


    // Part 2
    let mut a: u64 = START_VAL_A;
    let mut b: u64 = START_VAL_B;

    let mut count = 0;
    for _ in 0..NUM_STEPS_PART_2 {
        a = next_val(a, MUL_A, MUST_BE_MULTIPLE_OF_A);
        b = next_val(b, MUL_B, MUST_BE_MULTIPLE_OF_B);

        if a & 0b1111111111111111 == b & 0b1111111111111111 {
            count += 1;
        }
    }
    println!("{count}");
}


fn next_val(mut x: u64, multiplicator: u64, must_be_multiple_of: u64) -> u64 {
    loop {
        x = (x * multiplicator) % DIVISOR;
        if x % must_be_multiple_of == 0 {
            return x
        }
    }
}
