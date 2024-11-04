use std::fs;
use std::ops;

// Ideas stolen from https://www.redblobgames.com/grids/hexagons/

struct Hex {
    q: i32,
    r: i32,
    s: i32
}

impl ops::Sub<&Hex> for &Hex {
    type Output = Hex;
    fn sub(self, _rhs: &Hex) -> Hex {
        Hex {q: self.q - _rhs.q, r: self.r - _rhs.r, s: self.s - _rhs.s}
    }

}

impl Hex {
    fn mov(&mut self, direction: &str) {
        let (dq, dr, ds) = match direction {
            "n"  => (0, -1, 1),
            "ne" => (1, -1, 0),
            "se" => (1, 0, -1),
            "s"  => (0, 1, -1),
            "sw" => (-1, 1, 0),
            "nw" => (-1, 0, 1),
            _ => panic!("Unkown move direction '{direction}'")
        };
        self.q += dq;
        self.r += dr;
        self.s += ds;
    }

    fn distance(&self, other: &Hex) -> i32 {
        let vec = self - other;
        (vec.q.abs() + vec.r.abs() + vec.s.abs()) / 2
    }

    fn distance_to_zero(&self) -> i32 {
        self.distance(&Hex {q: 0, r: 0, s: 0})
    }
}

fn main() {
    let mut pos = Hex {q: 0, r: 0, s: 0};
    let mut dist_max = 0;

    for dir in fs::read_to_string("input.txt").unwrap().trim().split(",") {
        pos.mov(dir);
        let dist = pos.distance_to_zero();
        if dist > dist_max {
            dist_max = dist;
        }
    }
    println!("Last distance: {}, maximum distance during travel: {}", pos.distance_to_zero(), dist_max);
}
