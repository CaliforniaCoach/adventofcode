import itertools
import math
import sys
import sympy

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


################ Part 2 #######################

def calc_intersection_time(an, bn, cn, dan, dbn, dcn, xn, yn, zn, dxn, dyn, dzn):
	solution = sympy.solve([f.subs([
		(x, xn), (y, yn), (z, zn), (dx, dxn), (dy, dyn), (dz, dzn),
		(a, an), (b, bn), (c, cn), (da, dan), (db, dbn), (dc, dcn)]) for f in hit_formulas], t)
	return solution[t] if t in solution else None


def check(db_num, dc_num):
	global grains, solutions, db, dc
	final_solutions = {key: val.subs([(db, db_num), (dc, dc_num)]) for key, val in solutions.items()}
	an = final_solutions[a]
	bn = final_solutions[b]
	cn = final_solutions[c]
	dan = final_solutions[da]
	t1n = final_solutions[t1]
	t2n = final_solutions[t2]

	if an.is_Integer and bn.is_Integer and cn.is_Integer and dan.is_Integer and t1n.is_nonnegative and t2n.is_nonnegative:
		print('Possible solution found: ', an, bn, cn, dan, db_num, dc_num)
		for i, grain in enumerate(grains):
			t = calc_intersection_time(an, bn, cn, dan, db_num, dc_num, *grain)
			if t == None or t.is_negative:
				print(f'   Grain {i} does not match')
				break
		else:
			print('This is a proper solution!')
			for grain in grains:
				t = calc_intersection_time(an, bn, cn, dan, db_num, dc_num, *grain)
				print(f'Rock hits grain {grain} at time {t}')
			print()
			print('Sum:', an + bn + cn)
			sys.exit(0)


x1, y1, z1, x2, y2, z2, x3, y3, z3, dx1, dy1, dz1, dx2, dy2, dz2, dx3, dy3, dz3, a, b, c, da, db, dc = sympy.symbols('x1, y1, z1, x2, y2, z2, x3, y3, z3, dx1, dy1, dz1, dx2, dy2, dz2, dx3, dy3, dz3, a, b, c, da, db, dc')
t1, t2, t3 = sympy.symbols('t1, t2, t3')

# result = sympy.solve([
# 	x1 + t1 * dx1 - a - t1 * da,
# 	y1 + t1 * dy1 - b - t1 * db,
# 	z1 + t1 * dz1 - c - t1 * dc,
# 	
# 	x2 + t2 * dx2 - a - t2 * da,
# 	y2 + t2 * dy2 - b - t2 * db,
# 	z2 + t2 * dz2 - c - t2 * dc,
# 	
# 	x3 + t3 * dx3 - a - t3 * da,
# 	y3 + t3 * dy3 - b - t3 * db,
# 	z3 + t3 * dz3 - c - t3 * dc,
# 
# 	], [a, b, c, da, db, dc, t1, t2, t3], dict=True)
# 
# print(result)
# sys.exit(0)


# Do initial calculations with first two grains
grain1 = grains[0]
grain2 = grains[1]

formulas = [
	x1 + t1 * dx1 - a - t1 * da,
	y1 + t1 * dy1 - b - t1 * db,
	z1 + t1 * dz1 - c - t1 * dc,

	x2 + t2 * dx2 - a - t2 * da,
	y2 + t2 * dy2 - b - t2 * db,
	z2 + t2 * dz2 - c - t2 * dc,

#	x3 + t3 * dx3 - a - t3 * da,
#	y3 + t3 * dy3 - b - t3 * db,
#	z3 + t3 * dz3 - c - t3 * dc
]

solutions = sympy.solve(formulas, [a, b, c, da, t1, t2], dict=True)
if len(solutions) != 1:
	raise
solutions = solutions[0]

solutions = {
	key: val.subs(
		[
			(x1, grain1[0]), (y1, grain1[1]), (z1, grain1[2]),
			(x2, grain2[0]), (y2, grain2[1]), (z2, grain2[2]),
			(dx1, grain1[3]), (dy1, grain1[4]), (dz1, grain1[5]),
			(dx2, grain2[3]), (dy2, grain2[4]), (dz2, grain2[5]),
		]) for key, val in solutions.items()
	} 

x, y, z, dx, dy, dz, t = sympy.symbols('x, y, z, dx, dy, dz, t')

hit_formulas = [
	x + t * dx - a - t * da,
	y + t * dy - b - t * db,
	z + t * dz - c - t * dc,
]

max_velocity_abs = 1
while True:
	print('Scanning max velocity', max_velocity_abs)
	db_numeric = -max_velocity_abs
	for dc_numeric in range(-max_velocity_abs, max_velocity_abs + 1):
		check(db_numeric, dc_numeric)
	db_numeric = max_velocity_abs
	for dc_numeric in range(-max_velocity_abs, max_velocity_abs + 1):
		check(db_numeric, dc_numeric)
	dc_numeric = -max_velocity_abs
	for db_numeric in range(-max_velocity_abs, max_velocity_abs + 1):
		check(db_numeric, dc_numeric)
	dc_numeric = max_velocity_abs
	for db_numeric in range(-max_velocity_abs, max_velocity_abs + 1):
		check(db_numeric, dc_numeric)
	max_velocity_abs += 1
