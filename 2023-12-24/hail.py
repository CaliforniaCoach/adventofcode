import itertools
import math
import sys


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


###################### Part 2 ########################

def grain_pos_after_t(grain, t):
	grain_x = grain[0] + grain[3] * t
	grain_y = grain[1] + grain[4] * t
	grain_z = grain[2] + grain[5] * t
	return (grain_x, grain_y, grain_z)


def calc_speed(grain1, grain2, t1, t2):
	if (grain1[0] - grain2[0]) % (t1 - t2) != 0:
		return None
	if (grain1[1] - grain2[1]) % (t1 - t2) != 0:
		return None
	if (grain1[2] - grain2[2]) % (t1 - t2) != 0:
		return None

	dx = (grain1[0] - grain2[0]) // (t1 - t2)
	dy = (grain1[1] - grain2[1]) // (t1 - t2)
	dz = (grain1[2] - grain2[2]) // (t1 - t2)
	return (dx, dy, dz)


def calc_pos(grain, speed, t):
	x = grain[0] - speed[0] * t
	y = grain[1] - speed[1] * t
	z = grain[2] - speed[2] * t
	return (x, y, z)


def calc_intersection_time(grain1, speed1, grain2, speed2):
	if speed1[0] == speed2[0]:
		if grain1[0] != grain2[0]:
			return None
		if speed1[1] == speed2[1]:
			if grain2[1] != grain2[1]:
				return None
			if speed1[2] == speed2[2]:
				return None
			if (grain1[2] - grain2[2]) % (speed1[2] - speed2[2]) != 0:
				return None
			return -(grain1[2] - grain2[2]) // (speed1[2] - speed2[2])
		if (grain1[1] - grain2[1]) % (speed1[1] - speed2[1]) != 0:
			return None
		t = -(grain1[1] - grain2[1]) // (speed1[1] - speed2[1])
	elif (grain1[0] - grain2[0]) % (speed1[0] - speed2[0]) != 0:
		return None
	else:
		t = -(grain1[0] - grain2[0]) // (speed1[0] - speed2[0])

	y1 = grain1[1] + speed1[1] * t
	y2 = grain2[1] + speed2[1] * t
	z1 = grain1[2] + speed1[2] * t
	z2 = grain1[2] + speed1[2] * t

	if y1 != y2 or z1 != z2:
		return None
	else:
		return t


grains_backup = list(grains)
grains = grains[:3]

t2 = 2
while True:
	t1 = 1
	print(f'Searching for {t1=}, {t2=} ...')
	while t1 < t2:
		# print(f'Searching for {t1=}, {t2=} ...')
		for i, (grain1, grain2) in enumerate(itertools.product(grains, repeat=2)):
			# Assume rock hits grain 1 after t1 and grain 2 after t2 -> calc rock pos and speed
			grain1_pos_t1 = grain_pos_after_t(grain1, t1)
			grain2_pos_t2 = grain_pos_after_t(grain2, t2)
			rock_speed = calc_speed(grain1_pos_t1, grain2_pos_t2, t1, t2)
			if rock_speed != None:
				rock_pos = calc_pos(grain1_pos_t1, rock_speed, t1)
				# Test
				for grain in grains:
					t = calc_intersection_time(rock_pos, rock_speed, grain[0:3], grain[3:])
					if t == None:
						break
				else:
					print(f'Solution found with {t1=}, {t2=}: pos {rock_pos}, speed {rock_speed}; solution: {sum(rock_pos)}')
		t1 += 1
	t2 += 1
