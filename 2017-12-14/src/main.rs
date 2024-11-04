const KEY: &str = "wenycdww";
const SIZE: usize = 128;

fn main() {
    let mut count = 0;
    let mut hashes: Vec<u128> = Vec::new();
    for i in 0..SIZE {
        let digest = &(KEY.to_string() + "-" + &i.to_string());
        print!("{digest} -> ");
        let mut hash = knot_hash(digest);
        println!("{:0128b}", hash);
        hashes.push(hash);
        while hash != 0 {
            count += hash & 1;
            hash >>= 1;
        }
    }
    println!("Number of used squares: {count}");

    let mut region_count = 0;
    for y in 0..hashes.len() {
        for x in 0..128 {
            if (1 << (128 - 1 - x)) & hashes[y] > 0 {
                fill_with_zeros(&mut hashes, x, y as i32);
                region_count += 1;
            }
        }
    }
    println!("Number of regions: {region_count}");
}


fn knot_hash(input: &str) -> u128 {
    let mut list = [0; 256];
    for i in 1..list.len() {
        list[i] = i;
    }
    let mut cur: usize = 0;
    let mut skipsize = 0;
    let mut seq: Vec<u8> = input.bytes().collect();
    for x in [17, 31, 73, 47, 23] {
        seq.push(x);
    }

    for _ in 0..64 {
        for length in &seq {
            for i in 0..(length / 2) as usize {
                let swap = list[(cur + i) % list.len()];
                list[(cur + i) % list.len()] = list[(cur + (*length as usize - i - 1)) % list.len()];
                list[(cur + (*length as usize - i - 1)) % list.len()] = swap;
            }
            cur = (cur + *length as usize + skipsize) % list.len();
            skipsize += 1;
        }
    }

    let mut hash = 0u128;
    for group in 0..16 {
        let mut val = list[group * 16];
        for i in 1..16 {
            val ^= list[group * 16 + i];
        }
        hash = (hash << 8) | (val as u128);
    }

    hash
}


fn fill_with_zeros(field: &mut Vec<u128>, x: i32, y: i32) {
    let _y = y as usize;
    if y < 0 || _y >= field.len() || x < 0 || x >= 128 || field[_y] & ((1 << (128 - 1 - x))) == 0 {
        return
    }

    field[_y] &= !((1 << (128 - 1 - x)));
    fill_with_zeros(field, x + 1, y);
    fill_with_zeros(field, x - 1, y);
    fill_with_zeros(field, x, y + 1);
    fill_with_zeros(field, x, y - 1);
}
