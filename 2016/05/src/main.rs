use md5;

const DOOR_ID: &str = "reyedfim";

fn print_array(arr: [char; 8]) {
    let str:String = arr.iter().collect();
    println!("{str}");
}

fn main() {
    let mut password1 = String::new();
    let mut password2 = ['x', 'x', 'x', 'x', 'x', 'x', 'x', 'x'];
    let mut i = 0;
    'main: loop {
        let concat = DOOR_ID.to_owned() + &i.to_string();
        let digest = format!("{:x}", md5::compute(concat));
        i += 1;
        if digest.starts_with("00000") {
            println!("{digest}");
            if password1.len() < 8 {
                password1.push(digest.chars().nth(5).unwrap());
            }
            let pos = digest.chars().nth(5).unwrap().to_digit(16).unwrap() as usize;
            if pos <= 7 && password2[pos] == 'x' {
                password2[pos] = digest.chars().nth(6).unwrap();
                print_array(password2);
            }

            if password1.len() < 8 {
                continue 'main;
            }
            for chr in password2 {
                if chr == 'x' {
                    continue 'main;
                }
            }
            break;
        }
    }
    println!("{password1}");
    print_array(password2);
}
