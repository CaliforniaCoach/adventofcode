import sys
import itertools


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


count = 0

############################## Part 2 #################################


def test(springs, numbers, pos, number_index, count):
	if number_index >= len(numbers):
		if '#' not in springs[pos:]:
			return count + 1

	while pos < len(springs):
		endpos = min(pos + numbers[number_index], len(springs) - 1)
		if springs[pos] == '#':
			for i in range(pos + 1, endpos):
				if springs[i] == '.':
					return count
			if springs[endpos] == '#':
				return count
			return test(springs, numbers, endpos + 1, number_index + 1, count)
		elif springs[pos] == '?':
			for i in range(pos + 1, endpos):
				if springs[i] == '.':
					break
			else:
				if springs[endpos] != '#':
					count = test(springs, numbers, endpos + 1, number_index + 1, count)
		pos += 1

	return count


total1 = 0
total2 = 0
count = 1
with open('test.txt') as reader:
	for line in reader.readlines():
		springs, numbers = line.split()
		springs = list(springs)
		numbers = [int(x) for x in numbers.split(',')]

		total1 += test(springs + ['.'], numbers, 0, 0, 0)

		springs = (springs + ["?"]) * 4 + springs
		numbers = numbers * 5

		total2 += test(springs + ['.'], numbers, 0, 0, 0)

		print(count, "->", total1, total2)
		count += 1
sys.exit(0)


############## Former brute force method, not used ##################

with open('test.txt') as reader:
	for line in reader.readlines():
		springs, numbers = line.split()
		numbers = [int(x) for x in numbers.split(',')]
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

print(count)

