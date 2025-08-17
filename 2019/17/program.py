import sys

VIDEO_FEED = "n"

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

DIRCHAR = {"^": (0, -1), ">": (1, 0), "v": (0, 1), "<": (-1, 0)}

class Maze:
    def __init__(self, prg):
        self.maze_map = []
        self.prg = tuple(prg)
        self.startpos = None

        self.read()


    def read(self):
        _, _, _, output, status = run(self.prg, 0, 0, ())
        assert status == PRG_TERMINATED
        line = []
        for char in output:
            if char == 10:
                if len(line) > 0:
                    self.maze_map.append("".join(line))
                line = []
            else:
                if chr(char) in ("^", ">", "v", "<"):
                    self.startpos = (len(line), len(self.maze_map))
                line.append(chr(char))
        if len(line) > 0:
            self.maze_map.append("".join(line))

    def print(self):
        for line in self.maze_map:
            print(line)

    def find_intersections(self):
        intersections = []
        for y in range(1, len(self.maze_map) - 1):
            for x in range(1, len(self.maze_map[y]) - 1):
                if all([self.maze_map[y + dy][x + dx] == '#' for dx, dy in ((0, 0), (1, 0), (-1, 0),
                                                                            (0, 1), (0, -1))]):
                    intersections.append((x, y))
        return intersections

    def calc_path_instructions(self):
        path = []
        pos = self.startpos
        cur_dir = DIRCHAR[self.get_cell(pos)]
        while True:
            command, cur_dir = self.calc_turn(pos, cur_dir)
            if not command:
                return path
            path.append(command)
            steps = 0
            while self.get_cell((pos[0] + cur_dir[0], pos[1] + cur_dir[1])) == "#":
                steps += 1
                pos = (pos[0] + cur_dir[0], pos[1] + cur_dir[1])
            path.append(str(steps))

    def get_cell(self, pos):
        x, y = pos
        if y < 0 or y >= len(self.maze_map) or x < 0 or x >= len(self.maze_map[y]):
            return None
        return self.maze_map[y][x]
    
    def calc_turn(self, pos, initial_dir):
        for dir in DIRCHAR.values():
            # Don't turn around :-)
            if dir[0] == -initial_dir[0] and dir[1] == -initial_dir[1]:
                continue
            if self.get_cell((pos[0] + dir[0], pos[1] + dir[1])) == "#":
                if (dir[0] == initial_dir[1] and dir[1] == -initial_dir[0]):
                    return "L", dir
                elif (dir[0] == -initial_dir[1] and dir[1] == initial_dir[0]):
                    return "R", dir
                else:
                    raise
        return None, None

def replace_all(list, sequence, replace_with):
    length = len(sequence)
    i = 0
    while i + length <= len(list):
        if list[i:i + length] == sequence:
            list = list[0:i] + [replace_with] + list[i + length:]
        i += 1
    return list

def find_longest_common_pattern(instr, *exclude):
    results = []
    for length in range(len(instr) // 2, 0, -2):
        for i in range(0, len(instr) - length * 2, 2):
            sequence = instr[i:i + length]
            if len(",".join(map(str, sequence))) > 20 or sequence in exclude:
                continue
            for k in range(i + length, len(instr) - length, 2):
                if sequence == instr[k:k + length]:
                    results.append((replace_all(instr, sequence, "X"), sequence))
                    break
    return min(results, key=lambda result: len(result[0]))


sys.setrecursionlimit(100000)
with open("input.txt") as reader:
    prg = [int(x.strip()) for x in reader.readline().split(",")]
maze = Maze(prg)
# maze.print()
intersections = maze.find_intersections()
print("Part 1 solution: ", sum(map(lambda pt: pt[0] * pt[1], intersections)))

instr = maze.calc_path_instructions()
# print(instr)
# print()
_, a = find_longest_common_pattern(instr)
_, b = find_longest_common_pattern(instr, a)
_, c = find_longest_common_pattern(instr, a, b)

instr = replace_all(instr, a, "A")
instr = replace_all(instr, b, "B")
instr = replace_all(instr, c, "C")

# print(instr)
# print(a)
# print(b)
# print(c)

input_text = chr(10).join([",".join(map(str, seq)) for seq in (instr, a, b, c)]) + chr(10)
input_text += VIDEO_FEED + chr(10)
print(input_text)

prg[0] = 2
_, _, _, output, status = run(tuple(prg), 0, 0, tuple(map(ord, input_text)))
print("".join(map(chr, output[:-1])))

print("Part 2 solution:", output[-1])
