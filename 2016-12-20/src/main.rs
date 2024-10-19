use std::fs;

fn main() {
    let mut ranges: Vec<(u32, u32)> = vec!((0, !0));
    for line in fs::read_to_string("input.txt").unwrap().lines() {
        let entry: Vec<u32> = line.split('-').map(|x| x.parse::<u32>().unwrap()).collect();
        let mut i = 0;
        while i < ranges.len() && ranges[i].1 < entry[0] {
            i += 1;
        }
        while i < ranges.len() && entry[1] >= ranges[i].0 {
            if entry[0] > ranges[i].0 {
                let range_low = ranges[i].0;
                let range_high = ranges[i].1;
                ranges[i] = (range_low, entry[0] - 1);
                if entry[1] < range_high {
                    ranges.insert(i + 1, (entry[1] + 1, range_high));
                    i += 1;
                }
                i += 1;
            } else {
                if entry[1] >= ranges[i].0 {
                    if entry[1] >= ranges[i].1 {
                        ranges.remove(i);
                    } else {
                        ranges[i] = (entry[1] + 1, ranges[i].1);
                        i += 1;
                    }
                } else {
                    i += 1;
                }
            }
        }
    }
    println!("Lowest allowed address: {}", ranges.first().unwrap().0);

    let mut num = 0;
    for range in ranges {
        num += range.1 - range.0 + 1;
    }
    println!("Number of allowed addresses: {}", num);
}
