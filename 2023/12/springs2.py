import time


def test(springs, numbers, pos, number_index, count):
	if number_index >= len(numbers):
		if '#' not in springs[pos:]:
			return count + 1
		
	if sum(numbers[number_index:]) + len(numbers) - number_index - 1 > len(springs) - pos:
		return count
	
	if number_index >= len(numbers):
		return count

	while pos < len(springs):
		endpos = pos + numbers[number_index]
		if endpos > len(springs):
			return count
		if springs[pos] == '#':
			for i in range(pos + 1, endpos):
				if springs[i] == '.':
					return count
			if endpos < len(springs) and springs[endpos] == '#':
				return count
			return test(springs, numbers, endpos + 1, number_index + 1, count)
		elif springs[pos] == '?':
			for i in range(pos + 1, endpos):
				if springs[i] == '.':
					break
			else:
				if endpos >= len(springs) or springs[endpos] != '#':
					count = test(springs, numbers, endpos + 1, number_index + 1, count)
		pos += 1

	return count


def calc_pure_question_marks_sequence(sequence, numbers):
	n = len(sequence) - (sum(numbers) + len(numbers) - 1) + 1
	num = 1
	for i in range(len(numbers)):
		num = int(num * (n + i) / (i + 1))
	return num


def test_sequence(sequences, seq_index, numbers, num_index, count):
	if seq_index >= len(sequences):
		if num_index >= len(numbers):
			return 1
		else:
			return 0
	
	if sum(numbers[num_index:]) > sum([sequence[0] for sequence in sequences[seq_index:]]):
		return 0

	sequence_len = sequences[seq_index][0]
	sequence = sequences[seq_index][1]
	max_possible_index = num_index
	while True:
		if sum(numbers[num_index:max_possible_index]) + max_possible_index - num_index - 1 > sequence_len:
			max_possible_index -= 1
			break
		elif max_possible_index >= len(numbers):
			break
		max_possible_index += 1
	contains_hashes = '#' in sequence
	count = 0
	for i in range(num_index + 1 if contains_hashes else num_index, max_possible_index + 1):
		possibilities_this_sequence = test_sequence(sequences, seq_index + 1, numbers, i, count)
		if possibilities_this_sequence > 0:
			sequence_numbers = numbers[num_index : i]
			# If sequence consists only of "?", take a shortcut
			if sequence == ['?'] * len(sequence):
				possibilities_this_sequence *= calc_pure_question_marks_sequence(sequence, sequence_numbers)
			else:
				possibilities_this_sequence *= test(sequence, sequence_numbers, 0, 0, 0)
			count += possibilities_this_sequence

	return count


def process_line(springs, numbers):
	no_point_sequences = []
	pos_sequence_start = 0
	while True:
		while springs[pos_sequence_start] == '.':
			pos_sequence_start += 1
			if pos_sequence_start >= len(springs):
				break
		else:
			pos_sequence_end = pos_sequence_start
			while springs[pos_sequence_end] != '.':
				if pos_sequence_end >= len(springs):
					break
				pos_sequence_end += 1
			else:
				no_point_sequences.append((pos_sequence_end - pos_sequence_start, springs[pos_sequence_start:pos_sequence_end]))
				pos_sequence_start = pos_sequence_end
				continue
		break

	count = test_sequence(no_point_sequences, 0, numbers, 0, 1)

	return count


total = 0
count = 1
starttime = time.time()
with open('input.txt') as reader:
	for line in reader.readlines():
		springs, numbers = line.split()
		springs = list(springs)
		numbers = [int(x) for x in numbers.split(',')]

		possibilities = process_line(springs + ['.'], numbers)
		total += possibilities

		print(count, "->", possibilities, ", sum: ", total, "      seconds elapsed: ", time.time() - starttime)
		count += 1

print()

total = 0
count = 1
with open('input.txt') as reader:
	for line in reader.readlines():
		springs, numbers = line.split()
		springs = list(springs)
		numbers = [int(x) for x in numbers.split(',')]

		springs = (springs + ["?"]) * 4 + springs
		numbers = numbers * 5

		possibilities = process_line(springs + ['.'], numbers)
		total += possibilities

		print(count, "->", possibilities, ", sum: ", total, "      seconds elapsed: ", time.time() - starttime)
		count += 1
