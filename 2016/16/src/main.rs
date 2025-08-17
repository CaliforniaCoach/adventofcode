fn main() {
    solve("10000", 20);
    solve("10111011111001111", 272);
    solve("10111011111001111", 35651584);
}


fn solve(initial: &str, length: usize) {
    let mut data: Vec<bool> = Vec::new();
    for chr in initial.chars() {
        data.push(match chr {'0' => false, '1' => true, _ => panic!("invalid char")});
    }

    while data.len() < length {
        let b: Vec<bool> = data.iter().rev().map(|x| !x).collect();
        data.push(false);
        data.extend(b);
    }

    while data.len() > length {
        data.pop();
    }

    while data.len() % 2 == 0 {
        let mut new_data: Vec<bool> = Vec::new();
        for i in 0..data.len() / 2 {
            let bit0 = data[i * 2];
            let bit1 = data[i * 2 + 1];
            let checksum_digit = bit0 && bit1 || !bit0 && !bit1;
            new_data.push(checksum_digit);
        }
        data = new_data;
    }

    let checksum: String = data.iter().map(|x| if *x {'1'} else {'0'}).collect();
    println!("{checksum}");
    println!();
}
