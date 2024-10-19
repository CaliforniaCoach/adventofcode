use md5;

const GRIDSIZE: i32 = 4;

fn main() {
    solve("hijkl");
    solve("ihgpwlah");
    solve("kglvqrro");
    solve("ulqzkmiv");

    solve("pslxynzg");
}


fn solve(passcode: &str) {
    if let Some(path) = next_room(0, 0, String::new(), None, passcode) {
        println!("Passcode {}: Min path: {}, Max path len: {}", passcode, path.0, path.1.len());
    } else {
        println!("Passcode {}: No solution found", passcode);
    }
}


fn next_room(x: i32, y: i32, path: String, min_max_path: Option<(String, String)>, passcode: &str) -> Option<(String, String)> {
    if x < 0 || x >= GRIDSIZE || y < 0 || y >= GRIDSIZE {
        return min_max_path
    }

    if x == GRIDSIZE - 1 && y == GRIDSIZE - 1 {
        if let Some(min_max_path) = min_max_path {
            return Some((
                if path.len() < min_max_path.0.len() {path.clone()} else {min_max_path.0},
                if path.len() > min_max_path.1.len() {path} else {min_max_path.1}
            ));
        } else {
            return Some((path.clone(), path));
        }
    }

    let digest = md5::compute(passcode.to_owned() + &path);

    let mut new_min_max_path = min_max_path;
    if digest[0] >> 4 >= 0xb {
        new_min_max_path = next_room(x, y - 1, path.clone() + "U", new_min_max_path, passcode);
    }
    if digest[0] & 0xf >= 0xb {
        new_min_max_path = next_room(x, y + 1, path.clone() + "D", new_min_max_path, passcode);
    }
    if digest[1] >> 4 >= 0xb {
        new_min_max_path = next_room(x - 1, y, path.clone() + "L", new_min_max_path, passcode);
    }
    if digest[1] & 0xf >= 0xb {
        new_min_max_path = next_room(x + 1, y, path.clone() + "R", new_min_max_path, passcode);
    }
    new_min_max_path
}
