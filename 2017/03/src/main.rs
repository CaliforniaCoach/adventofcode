use std::collections::HashMap;

const INPUT: u32 = 277678;

fn main() {
    for i in 1..101 {
        solve_for(i);
    }
    println!();
    println!("Part 1 solution:");
    solve_for(INPUT);

    println!();
    println!("Part 2 solution:");
    println!("{}", solve_part2_for(INPUT));
}


fn solve_for(number: u32) {
    // r = "ring no", n = number at bottom right of ring = hightest number of ring
    // n = (2r - 1) ^ 2
    // 2r - 1 = sqrt(n)
    // r = (sqrt(n) + 1) / 2
    let ring_no = (((number - 1) as f64).sqrt() as i32 + 1) / 2;
    let offset_on_ring = number as i32 - ((2 * ring_no - 1) * (2 * ring_no - 1) + 1);

    let ring_side_len = 2 * ring_no + 1;
    let offset_top_right = ring_side_len - 1 - 1;
    let offset_top_left = offset_top_right + ring_side_len - 1;
    let offset_bottom_left = offset_top_left + ring_side_len - 1;
    let offset_bottom_right = offset_bottom_left + ring_side_len - 1;
    let y_ring_start = -(ring_no - 1);
    let (dx, dy) = 
        if offset_on_ring <= offset_top_right {
            (ring_no, y_ring_start + offset_on_ring)
        } else if offset_on_ring <= offset_top_left {
            (ring_no - (offset_on_ring - offset_top_right) , ring_no)
        } else if offset_on_ring <= offset_bottom_left {
            (-ring_no, ring_no - (offset_on_ring - offset_top_left))
        } else if offset_on_ring <= offset_bottom_right {
            ((offset_on_ring - offset_bottom_left) - ring_no, ring_no)
        } else {
            panic!();
        };
    println!("{number} -> ring_no: {ring_no}, offset: {offset_on_ring}, dx: {dx}, dy: {dy}, dist: {}", dx.abs() + dy.abs());
}


fn solve_part2_for(number: u32) -> u32 {
     let mut field: HashMap<isize, HashMap<isize, u32>> = HashMap::new();
     field.insert(0, HashMap::new());
     field.get_mut(&0).unwrap().insert(0, 1);

     let mut ring_no = 1;

     loop {
        for y in -ring_no + 1..ring_no + 1 {
            let last_num = add_tile(&mut field, ring_no, y);
            if last_num > number {
                return last_num
            }
        }
        for x in (-ring_no..ring_no + 1).rev() {
            let last_num = add_tile(&mut field, x, ring_no);
            if last_num > number {
                return last_num
            }
        }
        for y in (-ring_no..ring_no + 1).rev() {
            let last_num = add_tile(&mut field, -ring_no, y);
            if last_num > number {
                return last_num
            }
        }
        for x in -ring_no..ring_no + 1 {
            let last_num = add_tile(&mut field, x, -ring_no);
            if last_num > number {
                return last_num
            }
        }
        ring_no += 1;
     }
}

fn add_tile(field: &mut HashMap<isize, HashMap<isize, u32>>, x: isize, y: isize) -> u32 {
    let mut sum = 0;
    for dx in -1..2 {
        for dy in -1..2 {
            if dx != 0 || dy != 0 {
                if field.contains_key(&(y + dy)) {
                    if field[&(y + dy)].contains_key(&(x + dx)) {
                        sum += field[&(y + dy)][&(x + dx)];
                    }
                }
            }
        }
    }
    field.entry(y).and_modify(|row| {row.insert(x, sum);}).or_insert(HashMap::new()).insert(x, sum);
    sum
}
