import sys


elves = []
search_directions = [
	((0, -1), ((-1, -1), (0, -1), (1, -1))),
	((0, 1), ((-1, 1), (0, 1), (1, 1))),
	((-1, 0), ((-1, -1), (-1, 0), (-1, 1))),
	((1, 0), ((1, -1), (1, 0), (1, 1)))
]

with open(sys.argv[1], 'r') as reader:
	for i, line in enumerate(reader):
		for k, char in enumerate(line.strip()):
			if char == '#':
				elves.append((k, i))

round = 0
while True:
	round += 1
	elves_next = []
	for elve in elves:
		for x, y in ((-1, -1), (-1, 0), (-1, 1), (0, -1), (0, 1), (1, -1), (1, 0), (1, 1)):
			if (elve[0] + x, elve[1] + y) in elves:
				for direction in search_directions:
					for testfield in direction[1]:
						if (elve[0] + testfield[0], elve[1] + testfield[1]) in elves:
							break
					else:
						elves_next.append((elve[0] + direction[0][0], elve[1] + direction[0][1]))
						break
				else:
					elves_next.append(elve)
				break
		else:
			# No elve in the surrounding 8 fields, stay where you are
			elves_next.append(elve)

	movements = 0
	for i, elve_next in enumerate(elves_next):
		if elves[i] != elve_next:
			if elves_next.count(elve_next) == 1:
				elves[i] = elve_next
				movements += 1

	if round % 10 == 0:
		print(f'Round {round}: Movements: {movements}')

	if movements == 0:
		print(f'No more movement after round {round}')
		break

	search_directions.append(search_directions.pop(0))

	if round == 10:
		min_x = min([elve[0] for elve in elves])
		max_x = max([elve[0] for elve in elves])
		min_y = min([elve[1] for elve in elves])
		max_y = max([elve[1] for elve in elves])
		print(f'Empty fields after 10 rounds: {(max_x - min_x + 1) * (max_y - min_y + 1) - len(elves)}')
