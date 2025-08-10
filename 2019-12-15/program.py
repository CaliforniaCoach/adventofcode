import sys

WAITING_FOR_INPUT = 0
LOOP_DETECTED = 1
PRG_TERMINATED = 2

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

DIR = {1: (0, -1), 2: (0, 1), 3: (-1, 0), 4: (1, 0)}

class Maze:
    def __init__(self, prg):
        self.maze_map = {}
        self.oxygen_location = None

        self.try_next_moves(prg, 0, 0, 0, 0, 0)

    def do_next_move(self, prg, pc, relbase, x, y, steps_from_start, move):
        x += DIR[move][0]
        y += DIR[move][1]
        if (x, y) in self.maze_map:
            val = self.maze_map[(x, y)]
            if val <= steps_from_start:
                # This includes walls, which have a val of -1
                return
        prg, pc, relbase, output, status = run(prg, pc, relbase, (move, ))
        assert status == WAITING_FOR_INPUT
        assert len(output) == 1
        if output[0] == 0:
            self.maze_map[(x, y)] = -1
        elif output[0] in (1, 2):
            self.maze_map[(x, y)] = steps_from_start
            if output[0] == 2:
                assert self.oxygen_location is None or self.oxygen_location == (x, y)
                self.oxygen_location = (x, y)
            self.try_next_moves(prg, pc, relbase, x, y, steps_from_start)
        else:
            raise

    def try_next_moves(self, prg, pc, relbase, x, y, steps_from_start):
        for next_move in range(1, 5):
            self.do_next_move(prg, pc, relbase, x, y, steps_from_start + 1, next_move)

    def print(self):
        ymin = min([pt[1] for pt in self.maze_map.keys()])
        ymax = max([pt[1] for pt in self.maze_map.keys()])
        xmin = min([pt[0] for pt in self.maze_map.keys()])
        xmax = max([pt[0] for pt in self.maze_map.keys()])
        for y in range(ymin, ymax + 1):
            for x in range(xmin, xmax + 1):
                v = self.maze_map.get((x, y), 0)
                print("S" if x == 0 and y == 0 else "#" if v < 0 else "." if v > 0 else " ", end="")
            print()

    def get_shortest_dist_to_oxygen(self):
        return self.maze_map[self.oxygen_location]

    def calc_longest_dist_from_oxygen(self):
        return self.spread(self.oxygen_location[0], self.oxygen_location[1], 0)

    def spread(self, x, y, steps_so_far):
        if self.maze_map[(x, y)] < 0:
            return steps_so_far - 1
        self.maze_map[(x, y)] = -1
        max_steps = 0
        for d in range(1, 5):
            max_steps = max(max_steps, self.spread(x + DIR[d][0], y + DIR[d][1], steps_so_far + 1))
        return max_steps


sys.setrecursionlimit(100000)
with open("input.txt") as reader:
    prg = [int(x.strip()) for x in reader.readline().split(",")]
maze = Maze(tuple(prg))
maze.print()
print(maze.get_shortest_dist_to_oxygen())
print(maze.calc_longest_dist_from_oxygen())
