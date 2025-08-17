import sys
import re

# Format: [<dirname>, <dict of subdirectories>, <parent>, <total_size>]
root = ['/', {}, None, 0]


with open(sys.argv[1], 'r') as reader:
	for line in (x.strip() for x in reader):
		e = line.split(' ')
		if e[0] == '$':
			if e[1] == 'cd':
				if e[2] == '/':
					cwd = root
				elif e[2] == '..':
					cwd = cwd[2]
				else:
					if e[2] not in cwd[1]:
						cwd[1][e[2]] = [e[2], {}, cwd, 0]
					cwd = cwd[1][e[2]]
			elif e[1] == 'ls':
				pass
			else:
				raise SyntaxError
		elif e[0] != 'dir':
			dir = cwd
			while dir != None:
				dir[3] += int(e[0])
				dir = dir[2]

print(root)
print()

total_below_100000 = 0

def rek(dir):
	global total_below_100000
	if dir[3] <= 100000:
		print(dir[0], dir[3])
		total_below_100000 += dir[3]
	for d in dir[1].values():
		rek(d)

rek(root)

print()
print(f'{total_below_100000=}')


########################### Part 2 ################################

smallest_dir_but_big_enough = (root[0], root[3])
need_to_delete = 30000000 - (70000000 - root[3])

def rek2(dir):
	global smallest_dir_but_big_enough
	global need_to_delete
	if dir[3] >= need_to_delete and dir[3] < smallest_dir_but_big_enough[1]:
		smallest_dir_but_big_enough = (dir[0], dir[3])
	for d in dir[1].values():
		rek2(d)

rek2(root)

print('To delete:', smallest_dir_but_big_enough)
