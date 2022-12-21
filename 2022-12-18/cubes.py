import sys
import itertools


cubes = set()

with open(sys.argv[1], 'r') as reader:
	for line in reader:
		coords = line.split(',')
		cubes.add((int(coords[0]), int(coords[1]), int(coords[2])))

num_covered_sides = 0

for cube1, cube2 in itertools.combinations(cubes, 2):
	if abs(cube1[0] - cube2[0]) + abs(cube1[1] - cube2[1]) + abs(cube1[2] - cube2[2]) == 1:
		num_covered_sides += 2

print(len(cubes) * 6 - num_covered_sides)

min_x = min([cube[0] for cube in cubes])
max_x = max([cube[0] for cube in cubes])
min_y = min([cube[1] for cube in cubes])
max_y = max([cube[1] for cube in cubes])
min_z = min([cube[2] for cube in cubes])
max_z = max([cube[2] for cube in cubes])


def check_and_append(stack, visited, x, y, z):
	if (x, y, z) not in cubes and (x, y, z) not in visited:
		stack.append((x, y, z))
		visited |= set(((x, y, z), ))
		return True
	else:
		return False


def find_way_out(start_x, start_y, start_z, visited=set()):
	stack = []
	check_and_append(stack, visited, start_x, start_y, start_z)
	while stack != []:
		x, y, z = stack.pop(0)
		if (x, y, z) in cubes:
			continue
		if x <= min_x or x >= max_x or y <= min_y or y >= max_y or z <= min_z or z >= max_z:
			return True
		check_and_append(stack, visited, x + 1, y, z)
		check_and_append(stack, visited, x - 1, y, z)
		check_and_append(stack, visited, x, y + 1, z)
		check_and_append(stack, visited, x, y - 1, z)
		check_and_append(stack, visited, x, y, z + 1)
		check_and_append(stack, visited, x, y, z - 1)
	return False


trapped_cubes = set()
for x in range(min_x + 1, max_x):
	for y in range(min_y + 1, max_y):
		for z in range(min_z + 1, max_z):
			if (x, y, z) not in cubes and not find_way_out(x, y, z, set(trapped_cubes)):
				trapped_cubes.add((x, y, z))

num_covered_trapped_sides = 0
for cube1, cube2 in itertools.combinations(trapped_cubes, 2):
	if abs(cube1[0] - cube2[0]) + abs(cube1[1] - cube2[1]) + abs(cube1[2] - cube2[2]) == 1:
		num_covered_trapped_sides += 2

print(len(cubes) * 6 - num_covered_sides - (len(trapped_cubes) * 6 - num_covered_trapped_sides))
