with open("input.txt") as reader:
    prg = [int(x.strip()) for x in reader.readline().split(",")]

def get_parameters(prg, pc, num_parameters):
    parameter_values = []
    opcode_ext = prg[pc] // 100
    for i in range(num_parameters):
        parameter_values.append(prg[pc + i + 1] if opcode_ext % 10 else prg[prg[pc + i + 1]])
        opcode_ext //= 10
    return parameter_values

def run(prg, input, part2):
    pc = 0
    while prg[pc] != 99:
        opcode = prg[pc] % 100
        if opcode == 1:
            p = get_parameters(prg, pc, 2)
            assert not prg[pc] // 10_000
            prg[prg[pc + 3]] = p[0] + p[1]
            pc += 4
        elif opcode == 2:
            p = get_parameters(prg, pc, 2)
            assert not prg[pc] // 10_000
            prg[prg[pc + 3]] = p[0] * p[1]
            pc += 4
        elif opcode == 3:
            assert not prg[pc] // 100
            prg[prg[pc + 1]] = input
            pc += 2
        elif opcode == 4:
            print(get_parameters(prg, pc, 1)[0])
            pc += 2
        elif not part2:
            raise
        elif opcode == 5:
            p = get_parameters(prg, pc, 2)
            if p[0]:
                pc = p[1]
            else:
                pc += 3
        elif opcode == 6:
            p = get_parameters(prg, pc, 2)
            if not p[0]:
                pc = p[1]
            else:
                pc += 3
        elif opcode == 7:
            p = get_parameters(prg, pc, 2)
            assert not prg[pc] // 10_000
            prg[prg[pc + 3]] = 1 if p[0] < p[1] else 0
            pc += 4
        elif opcode == 8:
            p = get_parameters(prg, pc, 2)
            assert not prg[pc] // 10_000
            prg[prg[pc + 3]] = 1 if p[0] == p[1] else 0
            pc += 4
        else:
            raise
print("Part 1:")
run(list(prg), 1, False)

print("\nTests:")
print("must output 1:")
run([3,9,8,9,10,9,4,9,99,-1,8], 8, True)
print("must output 0:")
run([3,9,8,9,10,9,4,9,99,-1,8], 7, True)

print("\nPart 2:")
run(list(prg), 5, True)
