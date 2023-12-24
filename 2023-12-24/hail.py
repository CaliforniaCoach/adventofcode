import itertools
import math

min = 200000000000000
max = 400000000000000

grains = []
with open("input.txt") as reader:
	for line in reader.readlines():
		tok = line.replace(',', ' ').split()
		grains.append((int(tok[0]), int(tok[1]), int(tok[2]), int(tok[4]), int(tok[5]), int(tok[6])))

count = 0

for grain1, grain2 in itertools.combinations(grains, 2):
	x1 = grain1[0]
	x2 = grain2[0]
	y1 = grain1[1]
	y2 = grain2[1]
	dx1 = grain1[3]
	dx2 = grain2[3]
	dy1 = grain1[4]
	dy2 = grain2[4]
	if (dx1 != 0 and dx2 != 0 and dy1 / dx1 != dy2 / dx2) or (dy1 != 0 and dy2 != 0 and dx1 / dy1 != dx2 / dy2):
		x = (y1 - y2 - x1 * (dy1 / dx1) + x2 * (dy2 / dx2)) / (dy2 / dx2 - dy1 / dx1)
		y = (x1 - x2 - y1 * (dx1 / dy1) + y2 * (dx2 / dy2)) / (dx2 / dy2 - dx1 / dy1)
		if math.copysign(1, (x - x1)) == math.copysign(1, dx1) and math.copysign(1, (x - x2)) == math.copysign(1, dx2):
			if (x >= min and x <= max and y >= min and y <= max):
				count += 1

print(count)
