with open("input.txt") as reader:
    prg = [int(x.strip()) for x in reader.readline().split(",")]

def run(prg, noun, verb):
    prg[1] = noun
    prg[2] = verb

    pc = 0
    while prg[pc] != 99:
        if prg[pc] == 1:
            prg[prg[pc + 3]] = prg[prg[pc + 1]] + prg[prg[pc + 2]]
        elif prg[pc] == 2:
            prg[prg[pc + 3]] = prg[prg[pc + 1]] * prg[prg[pc + 2]]
        else:
            return None
        pc += 4

    return prg[0]

print(run(list(prg), 12, 2))

for noun in range(100):
    for verb in range(100):
        if run(list(prg), noun, verb) == 19690720:
            print(100 * noun + verb)
