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

def calc_intersection_x_y(x01, y01, x02, y02, dx1, dy1, dx2, dy2):
	if dx1 == 0:
		x = x1
	elif dx2 == 0:
		x = x2
	elif dy1 / dx1 == dy2 / dx2:
		return None, None
	else:
		x = (y01 - y02 - x01 * (dy1 / dx1) + x02 * (dy2 / dx2)) / (dy2 / dx2 - dy1 / dx1)

	if dy1 == 0:
		y = y1
	elif dy2 == 0:
		y = y2
	elif dx1 / dy1 == dx2 / dy2:
		return None, None
	else:
		y = (x01 - x02 - y01 * (dx1 / dy1) + y02 * (dx2 / dy2)) / (dx2 / dy2 - dx1 / dy1)

	return x, y


def calc_intersection_time_int_only(grain1, speed1, grain2, speed2):
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


def calc_intersection_time_float(grain1, speed1, grain2, speed2):
	if speed1[0] == speed2[0]:
		if grain1[0] != grain2[0]:
			return None
		if speed1[1] == speed2[1]:
			if grain2[1] != grain2[1]:
				return None
			if speed1[2] == speed2[2]:
				return None
			return -(grain1[2] - grain2[2]) / (speed1[2] - speed2[2])
		t_div = speed1[1] - speed2[1]
		t = -(grain1[1] - grain2[1]) / t_div
		# t_rem = -(grain1[1] - grain2[1]) % t_div
	else:
		t_div = speed1[0] - speed2[0]
		t = -(grain1[0] - grain2[0]) / t_div
		# t_rem = -(grain1[0] - grain2[0]) % t_div

	y1 = grain1[1] + speed1[1] * t 
	y2 = grain2[1] + speed2[1] * t
	z1 = grain1[2] + speed1[2] * t
	z2 = grain1[2] + speed1[2] * t

	if abs(y1 - y2) > 0.1 or abs(z1 - z2) > 0.1:
		return None
	else:
		return t



for grain1, grain2 in itertools.combinations(grains, 2):
	x1 = grain1[0]
	x2 = grain2[0]
	y1 = grain1[1]
	y2 = grain2[1]
	dx1 = grain1[3]
	dx2 = grain2[3]
	dy1 = grain1[4]
	dy2 = grain2[4]
	x, y = calc_intersection_x_y(x1, y1, x2, y2, dx1, dy1, dx2, dy2)
	if (x != None and y != None):
		if math.copysign(1, (x - x1)) == math.copysign(1, dx1) and math.copysign(1, (x - x2)) == math.copysign(1, dx2):
			if (x >= min and x <= max and y >= min and y <= max):
				count += 1

print(count)




# lcm_x = 1
# lcm_y = 1
# lcm_z = 1
# for grain in grains:
# 	lcm_x = math.lcm(lcm_x, grain[3])
# 	lcm_y = math.lcm(lcm_y, grain[4])
# 	lcm_z = math.lcm(lcm_z, grain[5])


def check(da, db, dc):
	global x1, x2, y1, y2, z1, z2, dx1, dx2, dy1, dy2, dz1, dz2, grains
	# Rock start pos is on intersection of two lines starting at (x1, y1, z1)
	x, y = calc_intersection_x_y(x1, y1, x2, y2, dx1 - da, dy1 - db, dx2 - da, dy2 - db)
	if x != None and y != None and x % 1 < 0.1 and y % 1 < 0.1:
		other_x, z = calc_intersection_x_y(x1, z1, x2, z2, dx1 - da, dz1 - dc, dx2 - da, dz2 - dc)
		if other_x != None and z != None and other_x % 1 < 0.1 and z % 1 < 0.1 and abs(x - other_x) < 0.1:
			x = int(x)
			y = int(y)
			z = int(z)
			print('Possible solution found: ', x, y, z, da, db, dc)
			for i, grain in enumerate(grains):
				t = calc_intersection_time_float((x, y, z), (da, db, dc), (grain[:3]), grain[3:])
				if t == None:
					print(f'   Grain {i} does not match')
					break
			else:
				print('This is a proper solution!')
				for grain in grains:
					t = calc_intersection_time_float((x, y, z), (da, db, dc), (grain[:3]), grain[3:])
					print(f'Rock hits grain {grain} at time {t}')
				print()
				print('Sum:', x + y + z)
				sys.exit(0)



# Do initial calculations with first two grains
grain1 = grains[0]
grain2 = grains[1]
x1 = grain1[0]
x2 = grain2[0]
y1 = grain1[1]
y2 = grain2[1]
z1 = grain1[2]
z2 = grain2[2]
dx1 = grain1[3]
dx2 = grain2[3]
dy1 = grain1[4]
dy2 = grain2[4]
dz1 = grain1[5]
dz2 = grain2[5]

max_velocity_abs = 1
while True:
	print('Scanning max velocity', max_velocity_abs)
	da = -max_velocity_abs
	for db, dc in itertools.product(range(-max_velocity_abs, max_velocity_abs + 1), repeat=2):
		check(da, db, dc)
	da = max_velocity_abs
	for db, dc in itertools.product(range(-max_velocity_abs, max_velocity_abs + 1), repeat=2):
		check(da, db, dc)
	db = -max_velocity_abs
	for da, dc in itertools.product(range(-max_velocity_abs, max_velocity_abs + 1), repeat=2):
		check(da, db, dc)
	db = max_velocity_abs
	for da, dc in itertools.product(range(-max_velocity_abs, max_velocity_abs + 1), repeat=2):
		check(da, db, dc)
	dc = -max_velocity_abs
	for da, db in itertools.product(range(-max_velocity_abs, max_velocity_abs + 1), repeat=2):
		check(da, db, dc)
	dc = max_velocity_abs
	for da, db in itertools.product(range(-max_velocity_abs, max_velocity_abs + 1), repeat=2):
		check(da, db, dc)
	max_velocity_abs += 1

for x in range(min, max + 1):
	for grain in grains:
		if (x - grain[0]) % grain[3] != 0:
			break
	else:
		print(f'x matching all grains: {x}')
