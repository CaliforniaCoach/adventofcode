use std::fs;
use std::collections::HashMap;
use core::arch::global_asm;


fn main() {
    let (_, count) = run(0);
    println!("MUL called {count} times");

    unsafe {
        println!("Reg h contains {}", run_with_asm());
    }
}



enum Arg {
    Val(i128),
    Reg(String)
}

impl From<&str> for Arg {
    fn from(arg: &str) -> Self {
        if let Ok(val) = arg.parse() {
            Arg::Val(val)
        } else {
            Arg::Reg(arg.to_string())
        }
    }
}

impl Arg {
    fn val(&self, reg: &mut HashMap<String, i128>) -> i128 {
        match self {
            Arg::Val(val) => *val,
            Arg::Reg(r) => {
                if !reg.contains_key(r) {
                    reg.insert(r.clone(), 0);
                }
                reg[r]
            }
        }
    }
}

enum Cmd {
    Set(Arg, Arg),
    Sub(Arg, Arg),
    Mul(Arg, Arg),
    Jnz(Arg, Arg)
}


fn run(reg_a_start_val: i128) -> (HashMap<String, i128>, u128) {
    let mut prg: Vec<Cmd> = Vec::new();
    for line in fs::read_to_string("input.txt").unwrap().trim().lines() {
        let mut tok = line.split_whitespace();
        let cmd = tok.next().unwrap();
        let arg1 = tok.next().unwrap();
        let arg2 = tok.next().unwrap();
        prg.push(match cmd {
            "set" => Cmd::Set(Arg::from(arg1), Arg::from(arg2)),
            "sub" => Cmd::Sub(Arg::from(arg1), Arg::from(arg2)),
            "mul" => Cmd::Mul(Arg::from(arg1), Arg::from(arg2)),
            "jnz" => Cmd::Jnz(Arg::from(arg1), Arg::from(arg2)),
            _ => panic!("Unknown command '{line}'")
        });
    }

    let mut pc: [i128; 2] = [0; 2];
    let mut reg: Vec<HashMap<String, i128>> = vec![HashMap::new(), HashMap::new()];
    reg[0].insert(String::from("a"), reg_a_start_val);
    let thread = 0;

    let mut mul_count: u128 = 0;

    while pc[thread] >= 0 && (pc[thread] as usize) < prg.len() {
        let mut reg = &mut reg[thread];
        match &prg[pc[thread as usize] as usize] {
            Cmd::Set(arg1, arg2) => {
                let val = arg2.val(&mut reg);
                set(&mut reg, &arg1, val);
            },
            Cmd::Sub(arg1, arg2) => {
                let val = arg1.val(&mut reg) - arg2.val(&mut reg);
                set(&mut reg, &arg1, val);
            },
            Cmd::Mul(arg1, arg2) => {
                let val = arg1.val(&mut reg) * arg2.val(&mut reg);
                set(&mut reg, &arg1, val);
                mul_count += 1;
            },
            Cmd::Jnz(arg1, arg2) => {
                if arg1.val(&mut reg) != 0 {
                    pc[thread] += arg2.val(&mut reg) - 1;
                }
            },
        }
        pc[thread] += 1;
    }

    (reg[0].clone(), mul_count)
}


fn set(reg: &mut HashMap<String, i128>, reg_arg: &Arg, val: i128) {
    if let Arg::Reg(r) = reg_arg {
        reg.entry(r.to_string()).and_modify(|x| *x = val).or_insert(val);
    }
}


extern "C" {
    fn run_with_asm() -> i64;
}

global_asm!("
    run_with_asm:
        mov rax, 1
        mov rbx, 0
        mov rcx, 0
        mov rdx, 0
        mov r8, 0
        mov r9, 0
        mov r10, 0
        mov r11, 0

        mov rbx, 84
        mov rcx, rbx
        test rax, rax
        jnz line05
        jmp line09
    line05:
        imul rbx, 100
        sub rbx, -100000
        mov rcx, rbx
        sub rcx, -17000
    line09:
        mov r9, 1
        mov rdx, 2
    line11:
        mov r8, 2
    line12:
        mov r10, rdx
        imul r10, r8
        sub r10, rbx
        jnz line17
        mov r9, 0
    line17:
        sub r8, -1
        mov r10, r8
        sub r10, rbx
        jnz line12
        sub rdx, -1
        mov r10, rdx
        sub r10, rbx
        jnz line11
        test r9, r9
        jnz line27
        sub r11, -1
    line27:
        mov r10, rbx
        sub r10, rcx
        test r10, r10
        jnz line31
        jmp end
    line31:
        sub rbx, -17
        jmp line09
    end:
        mov rax, r11
        ret
");
