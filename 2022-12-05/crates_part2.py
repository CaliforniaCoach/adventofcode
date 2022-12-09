import sys
import re


with open(sys.argv[1], 'r') as reader:
	lines = reader.readlines()

for i, line in enumerate(lines):
	line = line.rstrip()
	if re.fullmatch('[ 0-9]*[0-9][ 0-9]*', line):
		matches = list(re.finditer('[0-9]+', line))
		num_stacks = len(matches)
		max
		break

stack = [list() for i in range(num_stacks)]

for level in range(i):
	line = lines[level]
	for stack_no in range(num_stacks):
		char = 1 + stack_no * 4
		if char < len(line) and re.match('[A-Z]', line[char]):
			# stack[stack_no].insert(0, line[char])
			stack[stack_no].append(line[char])

print(stack)

for line in lines[i + 2:]:
	m = re.match('move ([0-9]+) from ([0-9]+) to ([0-9]+)', line)
	num = int(m.group(1))
	stack_from = int(m.group(2))
	stack_to = int(m.group(3))
	crates = stack[stack_from - 1][0:num]
	# crates.reverse()
	stack[stack_to - 1] = crates + stack[stack_to - 1]
	stack[stack_from - 1] = stack[stack_from - 1][num:]

print(stack)

print(''.join([s[0] for s in stack]))
