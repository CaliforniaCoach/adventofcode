import sys
import functools
import time

WAITING_FOR_INPUT = 0
LOOP_DETECTED = 1
PRG_TERMINATED = 2

next_print = 0.0

def get_parameters(prg, pc, relbase, num_parameters):
    parameter_values = []
    opcode_ext = prg[pc] // 100
    for i in range(num_parameters):
        if opcode_ext % 10 == 0:
            parameter_values.append(prg[prg[pc + i + 1]])
        elif opcode_ext % 10 == 1:
            parameter_values.append(prg[pc + i + 1])
        elif opcode_ext % 10 == 2:
            parameter_values.append(prg[prg[pc + i + 1] + relbase])
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

@functools.lru_cache(100000)
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

    # inp = list(inp)
    # output = []

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

def play(prg, input):
    field = {}
    triple = []
    score = 0

    mem, pc, relbase, output, status = run(tuple(prg), 0, 0, tuple(input))
    if status == WAITING_FOR_INPUT:
        # Program ran out of input
        return None, None
    if status == LOOP_DETECTED:
        # Program is caught in a loop
        return set((0, 0))

    # for out in output:
    #     triple.append(out)
    #     if len(triple) == 3:
    #         if triple[0] == -1 and triple[1] == 0:
    #             score = triple[2]
    #         else:
    #             field[(triple[0], triple[1])] = triple[2]
    #         triple = []

    blocks = set()
    score = 0
    for i in range(0, len(output), 3):
        if output[i] == -1 and output[i + 1] == 0:
            score = output[i + 2]
        elif output[i + 2] == 2:
            blocks.add((output[i], output[i + 1]))
        elif output[i + 2] == 0:
            if (output[i], output[i + 1]) in blocks:
                blocks.remove((output[i], output[i + 1]))


    return blocks, score

def print_field(field):
    for y in range(min(map(lambda x: x[1], field)), max(map(lambda x: x[1], field)) + 1):
        for x in range(min(map(lambda x: x[0], field)), max(map(lambda x: x[0], field)) + 1):
            tile = field.get((x, y), 0)
            print({0: " ", 1: "#", 2: "-", 3: "_", 4: "o"}[tile], end="")
        print()

def count_blocks(field):
    return list(field.values()).count(2)


with open("input.txt") as reader:
    prg = [int(x.strip()) for x in reader.readline().split(",")]

blocks, score = play(prg, [])
print("Part 1 solution: ", len(blocks))

def try_next_move(prg, moves_so_far):
    blocks, score = play(prg, moves_so_far)
    if blocks is not None:
        if len(blocks) == 0:
            # We won!
            return moves_so_far, score
        else:
            # We failed
            global next_print
            if time.time() > next_print:
                print("Blocks: ", len(blocks), " Score: ", score)
                # print_field(result)
                print(moves_so_far)
                next_print = time.time() + 5.0
            return None, None
    else:
        # We did not provide enough joystick movement inputs
        for next_move in (-1, 0, 1):
            result_moves, score = try_next_move(prg, moves_so_far + [next_move])
            if result_moves != None:
                return result_moves, score
        return None, None

prg[0] = 2
sys.setrecursionlimit(1000)

# Tests
# result, score = play(prg, (-1, -1, -1, 1))
# print_field(result)

# result, score = play(prg, (-1, -1, -1, 1))
# print_field(result)

moves, score = try_next_move(prg, [])
print("Part 2 solution: ", score)
print(moves)
