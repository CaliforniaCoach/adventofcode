import sys

cycle = 1
x = 1
xlist = [1]
pixel = []

with open(sys.argv[1], 'r') as reader:
    for line in reader:
        cmd = line.rstrip().split(' ')
        if cmd[0] == 'noop':
            pixel.append('#' if abs((cycle - 1) % 40 - x) <= 1 else '.')
            cycle += 1
            xlist.append(x)
        elif cmd[0] == 'addx':
            pixel.append('#' if abs((cycle - 1) % 40 - x) <= 1 else '.')
            xlist.append(x)
            cycle += 1
            pixel.append('#' if abs((cycle - 1) % 40 - x) <= 1 else '.')
            xlist.append(x)
            x += int(cmd[1])
            cycle += 1

print(xlist)

output = (20, 60, 100, 140, 180, 220)
sum = 0

for cycle in output:
    print(f'Cycle {cycle} : Signal {xlist[cycle] * cycle}')
    sum += xlist[cycle] * cycle

print(f'{sum=}')

for i, pix in enumerate(pixel):
    if i % 40 == 0:
        print()
    print(pix, end='')
print()
