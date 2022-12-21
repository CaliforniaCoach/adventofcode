import sys
import numpy


ROCKS = (
	(((0,0), (1,0), (2,0), (3,0)), 4, 1),
	(((1,0), (0,1), (1,1), (2,1), (1,2)), 3, 3),
	(((0,0), (1,0), (2,0), (2,1), (2,2)), 3, 3),
	(((0,0), (0,1), (0,2), (0,3)), 1, 4),
	(((0,0), (1,0), (0,1), (1,1)), 2, 2)
)


def collision(chamber, rock, x, y):
	if x < 0 or x + rock[1] - 1 >= 7 or y < 0:
		return True
	for part_x, part_y in rock[0]:
		if chamber[y + part_y][x + part_x]:
			return True
	return False


def print_chamber(chamber, max_y):
	for y in range(max_y, -1, -1):
		print('|', end='')
		for field in chamber[y]:
			print('#' if field else '.', end='')
		print('|')
	print('+-------+')
	print()


def simulate_falling_rocks(chamber, highest_rock, total_num_rocks=0, start_gas_index=0):
	i_gas = start_gas_index
	num_rocks = 0
	while True:
		rock = ROCKS[num_rocks % len(ROCKS)]
		y = highest_rock + 3 + 1
		x = 2

		while True:
			# Sideways movement
			if gas[i_gas] == '>':
				if not collision(chamber, rock, x + 1, y):
					x += 1
			else:
				if not collision(chamber, rock, x - 1, y):
					x -= 1
			i_gas = (i_gas + 1) % len(gas)

			# Downwards movement
			if not collision(chamber, rock, x, y - 1):
				y -= 1
			else:
				for part_x, part_y in rock[0]:
					chamber[y + part_y][x + part_x] = True
				highest_rock = max(highest_rock, y + rock[2] - 1)
				break

		num_rocks += 1
		if total_num_rocks > 0:
			if num_rocks >= total_num_rocks:
				break
		else:
			if num_rocks % len(ROCKS) == 0:
				if i_gas == start_gas_index:
					break

	return highest_rock, num_rocks, i_gas


with open(sys.argv[1], 'r') as reader:
	gas = tuple(reader.readline().strip())


chamber = [[False] * 7 for i in range(100000)]

highest_rock, num_rocks, i_gas = simulate_falling_rocks(chamber, -1, 2022, 0)
print(f'After 2022 rocks: Tower {highest_rock + 1} tall')


rocks_to_drop = 1000000000000
chamber = [[False] * 7 for i in range(10000000)]

last_highest_rock, total_rocks, last_gas_index = simulate_falling_rocks(chamber, -1, 1000)

last_num_rocks = 0
last_delta_highest_rock = 0
while True:
	highest_rock, num_rocks, i_gas = simulate_falling_rocks(chamber, last_highest_rock, start_gas_index=last_gas_index)
	total_rocks += num_rocks
	delta_highest_rock = highest_rock - last_highest_rock
	if delta_highest_rock == last_delta_highest_rock and num_rocks == last_num_rocks and i_gas == last_gas_index:
		remaining_rocks = rocks_to_drop - total_rocks
		cycles = remaining_rocks // num_rocks
		total_highest_rock = highest_rock + cycles * delta_highest_rock
		total_rocks += cycles * num_rocks
		last_highest_rock = highest_rock
		highest_rock, num_rocks, i_gas = simulate_falling_rocks(chamber, last_highest_rock, rocks_to_drop - total_rocks, i_gas)
		total_highest_rock += highest_rock - last_highest_rock
		break
	last_highest_rock = highest_rock
	last_num_rocks = num_rocks
	last_delta_highest_rock = delta_highest_rock
	last_gas_index = i_gas

print(f'After {rocks_to_drop} rocks: Tower {total_highest_rock + 1} tall')
