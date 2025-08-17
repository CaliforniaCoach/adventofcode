import sys
import itertools
import time


def countConsecutiveDefects(springs):
	numbers = []
	count = 0
	for char in springs + [' ']:
		if char == '#':
			count += 1
		elif count > 0:
			numbers.append(count)
			count = 0
	return numbers


def process_line(springs, numbers, linenum):
	count = 0
	known_defects = springs.count('#')
	total_defects = sum(numbers)
	unknown_defects = total_defects - known_defects
	possible_defects = 0
	possible_defect_pos = []
	pos = 0
	while True:
		pos = springs.find('?', pos)
		if pos >= 0:
			possible_defect_pos.append(pos)
			possible_defects += 1
			pos += 1
		else:
			break

	for defects in itertools.combinations(range(possible_defects), unknown_defects):
		potential_springs = list(springs)
		for defect in defects:
			potential_springs[possible_defect_pos[defect]] = '#'
		if countConsecutiveDefects(potential_springs) == numbers:
			count += 1

	return count


linenum = 1
total = 0
with open('input.txt') as reader:
	for line in reader.readlines():
		springs, numbers = line.split()
		numbers = [int(x) for x in numbers.split(',')]
		count = process_line(springs, numbers, linenum)
		total += count
		print(linenum, "->", count, ", sum: ", total)
		linenum += 1

print()
print("Total: ", total)
print()

linenum = 1
total = 0
with open('input.txt') as reader:
	for line in reader.readlines():
		springs, numbers = line.split()
		numbers = [int(x) for x in numbers.split(',')]

		springs = (springs + '?') * 4 + springs
		numbers = numbers * 5

		count = process_line(springs, numbers, linenum)
		total += count
		print(linenum, "->", count, ", sum: ", total)
		linenum += 1

print()
print("Total: ", total)
