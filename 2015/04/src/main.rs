use md5;

const INPUT: &str = "yzbqklnj";

fn main() {
    let mut n = 1;
    'outer: loop {
        let mut key = format!("{INPUT}{n}");
        let digest = md5::compute(key);
        if digest[0] == 0 && digest[1] == 0 && digest[2] & 0xf0 == 0 {
            println!("Five zeros: {n}");
            if digest[2] == 0 {
                println!("Six zeros: {n}");
                break;
            }
        }
        n += 1;
    }
}
