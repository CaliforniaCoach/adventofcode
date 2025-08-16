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

def output_to_queue(queue, output):
    x255, y255 = None, None
    while output:
        address = output.pop(0)
        if address == 255:
            x255 = output.pop(0)
            y255 = output.pop(0)
        else:
            queue[address].append(output.pop(0))
            queue[address].append(output.pop(0))
    return x255, y255

with open("input.txt") as reader:
    prg = tuple(int(x.strip()) for x in reader.readline().split(","))

computer = []
queue = [[] for i in range(50)]
for i in range(50):
    mem, pc, relbase, output, status = run(prg, 0, 0, (i, ))
    assert status == WAITING_FOR_INPUT
    computer.append((mem, pc, relbase))
    output_to_queue(queue, output)

no_packet_received_by_255_yet = True
last_y255_received = None
last_y255_sent = None
while True:
    empty_queues = 0
    empty_outputs = 0
    for i in range(50):
        c = computer[i]
        inp = tuple(queue[i])
        if not inp:
            inp = (-1, )
            empty_queues += 1
        mem, pc, relbase, output, status = run(c[0], c[1], c[2], inp)
        assert status == WAITING_FOR_INPUT
        queue[i] = []
        computer[i] = (mem, pc, relbase)
        if not output:
            empty_outputs += 1
        x255, y255 = output_to_queue(queue, output)
        if x255 is not None:
            if no_packet_received_by_255_yet:
                print("Part 1:", y255)
                no_packet_received_by_255_yet = False
            last_x255_received = x255
            last_y255_received = y255
    if empty_queues == 50 and empty_outputs == 50:
        if last_y255_received == last_y255_sent:
            print("Part 2:", last_y255_received)
            break
        last_y255_sent = last_y255_received
        queue[0] = [last_x255_received, last_y255_received]
