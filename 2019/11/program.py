import itertools

with open("input.txt") as reader:
    prg = {i: int(x.strip()) for i, x in enumerate(reader.readline().split(","))}

def get_parameters(prg, pc, relbase, num_parameters):
    parameter_values = []
    opcode_ext = prg[pc] // 100
    for i in range(num_parameters):
        if opcode_ext % 10 == 0:
            parameter_values.append(prg.get(prg[pc + i + 1], 0))
        elif opcode_ext % 10 == 1:
            parameter_values.append(prg[pc + i + 1])
        elif opcode_ext % 10 == 2:
            parameter_values.append(prg.get(prg[pc + i + 1] + relbase, 0))
        else:
            raise
        opcode_ext //= 10
    return parameter_values

def wrt(mem, pc, relbase, write_param_pc_offset, value):
    instr = mem.get(pc, 0)
    mode = instr // 10 ** (1 + write_param_pc_offset)
    if mode == 0:
        mem[mem.get(pc + write_param_pc_offset, 0)] = value
    elif mode == 2:
        mem[mem.get(pc + write_param_pc_offset, 0) + relbase] = value
    else:
        raise

def run(computer_state, *input):
    prg = computer_state[0]
    pc = computer_state[1]
    relbase = computer_state[2]
    input_buffer = list(reversed(input))
    while prg.get(pc, 0) != 99:
        instr = prg.get(pc, 0)
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
            wrt(prg, pc, relbase, 1, input_buffer.pop())
            pc += 2
        elif opcode == 4:
            computer_state[1] = pc + 2
            return get_parameters(prg, pc, relbase, 1)[0]
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
            computer_state[2] = relbase
            pc += 2
        else:
            raise
    return None

def find_max_output(program, min_phase):
    max_output = 0
    for phases in itertools.permutations(range(min_phase, min_phase + 5)):
        prgs_with_pcs_and_relstates = [[dict(program), 0, 0] for i in range(5)]
        signal = 0
        for i, phase in enumerate(phases):
            signal = run(prgs_with_pcs_and_relstates[i], phase, signal)
        amp = 0
        while True:
            out = run(prgs_with_pcs_and_relstates[amp], signal)
            if not out:
                assert amp == 0
                break
            signal = out
            amp = (amp + 1) % 5
        max_output = max(max_output, signal)
    return max_output

def to_dict(*iterable_to_convert):
    return {i: x for i, x in enumerate(iterable_to_convert)}

def paint(prg, first_panel):
    prg_with_pc_and_relstate = [dict(prg), 0, 0]
    current_pos = (0, 0)
    current_dir = (0, -1)
    panels_painted = {current_pos: first_panel}
    while True:
        output = run(prg_with_pc_and_relstate, panels_painted.get(current_pos, 0))
        if output == None:
            break
        if output != 0 and output != 1:
            raise
        panels_painted[current_pos] = output
        output = run(prg_with_pc_and_relstate, output)
        if output == None:
            raise
        if output == 0:
            current_dir = (current_dir[1], -current_dir[0])
        elif output == 1:
            current_dir = (-current_dir[1], current_dir[0])
        else:
            raise
        current_pos = (current_pos[0] + current_dir[0], current_pos[1] + current_dir[1])
    return panels_painted

def print_panels(panels):
    for y in range(min(p[1] for p in panels), max(p[1] for p in panels) + 1):
        for x in range(min(p[0] for p in panels), max(p[0] for p in panels) + 1):
            print("##" if panels.get((x, y), 0) else "  ", end="")
        print()


print("Tests:")
print("Expected: 43210")
print(find_max_output(to_dict(3,15,3,16,1002,16,10,16,1,16,15,15,4,15,99,0,0), 0))
print("Expected: 139629729")
print(find_max_output(to_dict(3,26,1001,26,-4,26,3,27,1002,27,2,27,1,27,26,27,4,27,1001,28,-1,28,1005,28,6,99,0,0,5), 5))

print("\nPart 1:")
print(len(paint(prg, 0)))

print("\nPart 2:")
print_panels(paint(prg, 1))
