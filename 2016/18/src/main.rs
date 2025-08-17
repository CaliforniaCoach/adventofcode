use std::fs;

fn main() {
    solve_for(".^^.^.^^^^", 10);
    solve_for(&fs::read_to_string("input.txt").unwrap(), 40);
    solve_for(&fs::read_to_string("input.txt").unwrap(), 400000);
}


fn solve_for(first_row: &str, num: i32) {
    let mut row: Vec<bool> = first_row
        .trim()
        .chars()
        .into_iter()
        .map(|x| match x {'.' => false, '^' => true, _ => panic!("invalid character")})
        .collect();

        let mut num_safe_tiles = count_safe_tiles_in_row(&row);
        for _i in 0..num - 1 {
        let mut new_row: Vec<bool> = Vec::new();
        new_row.push(row[1]);
        for k in 1..row.len() - 1 {
            new_row.push((row[k - 1] && !row[k + 1]) || (!row[k - 1] && row[k + 1]));
        }
        new_row.push(row[row.len() - 2]);

        row = new_row;
        num_safe_tiles += count_safe_tiles_in_row(&row);
    }

    println!("{num_safe_tiles}");
}


fn count_safe_tiles_in_row(row: &Vec<bool>) -> i32 {
    let mut retval = 0;
    for tile in row {
        if !tile {
            retval += 1;
        }
    }
    retval
}
