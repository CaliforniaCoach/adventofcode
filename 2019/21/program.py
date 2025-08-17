import sys

SCRIPT1 = """

NOT A J
NOT B T
OR T J
NOT C T
OR T J
AND D J
WALK

"""

SCRIPT2 = """

NOT B J
OR T J
NOT C T
OR T J
AND D J
AND H J
NOT A T
OR T J
RUN

"""

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

_, _, _, output, _ = run(prg, 0, 0, tuple(map(ord, SCRIPT1.lstrip())))
print("************************ Part 1 ************************")
print("".join((chr(out) if out <= 255 else str(out) for out in output)))

_, _, _, output, _ = run(prg, 0, 0, tuple(map(ord, SCRIPT2.lstrip())))
print("\n************************ Part 2 ************************")
print("".join((chr(out) if out <= 255 else str(out) for out in output)))

