use std::fs;

fn main() {
    let content = fs::read_to_string("input.txt").expect("Something went wrong");
    let bytes = String::from(&content).into_bytes();

    let mut start = 0;
    let mut end: usize = 0;
    let mut lengths: [u32; 3] = [0, 0, 0];
    let mut idx: usize = 0;
    let mut area: u32 = 0;
    let mut ribbon: u32 = 0;

    while end < bytes.len() {
        if bytes[end] == b'x' || bytes[end] == b'\n' {
            let number: u32 = content[start..end].parse().expect("Error");
            lengths[idx] = number;
            start = end + 1;
            idx += 1;
            if idx == 3 {
                let l = lengths[0];
                let w = lengths[1];
                let h = lengths[2];
                area += 2 * (l * w + w * h + h * l);

                lengths.sort();
                area += lengths[0] * lengths[1];

                ribbon += 2 * (lengths[0] + lengths[1]) + l * w * h;

                idx = 0;
            }
        }
        end += 1;
    }
    println!("Paper: {area}, Ribbon: {ribbon}");
}
