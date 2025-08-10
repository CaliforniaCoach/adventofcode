import sys

SHIPSIZE = 100

WAITING_FOR_INPUT = 0
LOOP_DETECTED = 1
PRG_TERMINATED = 2

def get(mem, addr):
    while len(mem) <= addr:
        mem.append(0)
    return mem[addr]

def get_parameters(prg, pc, relbase, num_parameters):
    parameter_values = []
    opcode_ext = prg[pc] // 100
    for i in range(num_parameters):
        if opcode_ext % 10 == 0:
            parameter_values.append(get(prg, prg[pc + i + 1]))
        elif opcode_ext % 10 == 1:
            parameter_values.append(get(prg,pc + i + 1))
        elif opcode_ext % 10 == 2:
            parameter_values.append(get(prg, prg[pc + i + 1] + relbase))
        else:
            raise
        opcode_ext //= 10
    return parameter_values

def wrt(mem, pc, relbase, write_param_pc_offset, value):
    instr = mem[pc]
    mode = instr // 10 ** (1 + write_param_pc_offset)
    if mode == 0:
        addr = mem[pc + write_param_pc_offset]
    elif mode == 2:
        addr = mem[pc + write_param_pc_offset] + relbase
    else:
        raise
    while len(mem) <= addr:
        mem.append(0)
    mem[addr] = value

def run(mem, pc, relbase, inp):
    assert isinstance(mem, tuple)
    assert isinstance(inp, tuple)
    if len(inp) > 1:
        states_already_met = set()
        mem, pc, relbase, out, status = run(mem, pc, relbase, inp[:-1])
        if status != WAITING_FOR_INPUT:
            return mem, pc, relbase, out, status
        if (mem, pc, relbase) in states_already_met:
            print("Loop detected")
            return mem, pc, relbase, out, LOOP_DETECTED
        states_already_met.add((mem, pc, relbase))
        inp = [inp[-1]]
        output = list(out)
    else:
        inp = list(inp)
        output = []

    prg = list(mem)
    while prg[pc] != 99:
        instr = prg[pc]
        opcode = instr % 100
        if opcode == 1:
            p = get_parameters(prg, pc, relbase, 2)
            wrt(prg, pc, relbase, 3, p[0] + p[1])
            pc += 4
        elif opcode == 2:
            p = get_parameters(prg, pc, relbase, 2)
            wrt(prg, pc, relbase, 3, p[0] * p[1])
            pc += 4
        elif opcode == 3:
            if not inp:
                return tuple(prg), pc, relbase, output, WAITING_FOR_INPUT
            wrt(prg, pc, relbase, 1, inp.pop(0))
            pc += 2
        elif opcode == 4:
            output.append(get_parameters(prg, pc, relbase, 1)[0])
            pc += 2
        elif opcode == 5:
            p = get_parameters(prg, pc, relbase, 2)
            if p[0]:
                pc = p[1]
            else:
                pc += 3
        elif opcode == 6:
            p = get_parameters(prg, pc, relbase, 2)
            if not p[0]:
                pc = p[1]
            else:
                pc += 3
        elif opcode == 7:
            p = get_parameters(prg, pc, relbase, 2)
            wrt(prg, pc, relbase, 3, 1 if p[0] < p[1] else 0)
            pc += 4
        elif opcode == 8:
            p = get_parameters(prg, pc, relbase, 2)
            wrt(prg, pc, relbase, 3, 1 if p[0] == p[1] else 0)
            pc += 4
        elif opcode == 9:
            p = get_parameters(prg, pc, relbase, 1)
            relbase += p[0]
            pc += 2
        else:
            raise
    return tuple(prg), pc, relbase, output, PRG_TERMINATED

def is_in_beam(prg, x, y):
    _, _, _, output, _ = run(prg, 0, 0, (x, y))
    return output[0] > 0

def get_beam_range(prg, y, min_x_49, max_x_49):
    x_min = min_x_49 * y // 49
    x_max = max_x_49 * y // 49
    while is_in_beam(prg, x_min, y):
        x_min -= 1
    while not is_in_beam(prg, x_min, y):
        x_min += 1
    while is_in_beam(prg, x_max, y):
        x_max += 1
    while not is_in_beam(prg, x_max, y):
        x_max -= 1
    return x_min, x_max

with open("input.txt") as reader:
    prg = tuple(int(x.strip()) for x in reader.readline().split(","))

tractor_points = 0
x_min_49 = 0
x_max_49 = 0
for y in range(50):
    for x in range(50):
        _, _, _, output, _ = run(prg, 0, 0, (x, y))
        tractor_points += output[0]
        if output[0] == 1:
            if x_min_49 == 0:
                x_min_49 = x
            x_max_49 = x
assert x_min_49 > 0
assert not is_in_beam(prg, y, x_max_49 + 1)

print(tractor_points)

min_y = 10
max_y = 99999999999999
y = 10

while max_y - min_y >= 1:
    x_min, x_max = get_beam_range(prg, y, x_min_49, x_max_49)
    if x_max - x_min >= SHIPSIZE and is_in_beam(prg, x_max - SHIPSIZE + 1, y + SHIPSIZE - 1):
        max_y = y
        y = min_y + (y - min_y) // 2
        last_max_x = x_max
    else:
        min_y = y + 1
        y = min(y * 2, y + (max_y - y) // 2)

# It can happen that below a non-fitting <y>, there are still some fitting <y>s -> verify them
for y in range(min_y, min_y - 10, -1):
    x_min, x_max = get_beam_range(prg, y, x_min_49, x_max_49)
    if x_max - x_min >= SHIPSIZE and is_in_beam(prg, x_max - SHIPSIZE + 1, y + SHIPSIZE - 1):
        min_y = y
        last_max_x = x_max

print((last_max_x - SHIPSIZE + 1) * 10000 + min_y)
