import sys

OFFSET = {'R': (1, 0), 'L': (-1, 0), 'U': (0, 1), 'D': (0, -1)}

num_knots = int(sys.argv[2])

pos = [[0, 0] for i in range(num_knots)]

tail_positions = {(0, 0)}

def sign(x):
	return 1 if x > 0 else -1 if x < 0 else 0

with open(sys.argv[1], 'r') as reader:
	for line in reader:
		command = line.rstrip().split(' ')
		offs = OFFSET[command[0]]
		for i in range(int(command[1])):
			pos[0][0] += offs[0]
			pos[0][1] += offs[1]
			for k in range(1, num_knots):
				if abs(pos[k-1][0] - pos[k][0]) >= 2 or abs(pos[k-1][1] - pos[k][1]) >= 2:
					pos[k][0] += sign(pos[k-1][0] - pos[k][0])
					pos[k][1] += sign(pos[k-1][1] - pos[k][1])
			tail_positions.add(tuple(pos[num_knots - 1]))

print(len(tail_positions))
