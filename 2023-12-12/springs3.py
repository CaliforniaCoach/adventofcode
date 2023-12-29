import time
import itertools

class Hashblock:
	def __init__(self, pos, endpos):
		self.pos = pos
		self.endpos = endpos
		self.size = endpos - pos


def test_qm_block(numbers, hashblocks, qm_blocks, idx_hashblock, idx_number, offset_from_last):
	if idx_number >= len(numbers):
		if idx_hashblock >= len(hashblocks) and offset_from_last <= qm_blocks[idx_hashblock]:
			return 1
		else:
			return 0
	
	if idx_hashblock == len(hashblocks):
		remaining_numbers = numbers[idx_number:]
		tiles_available = qm_blocks[idx_hashblock] - 1 - offset_from_last
		tiles_needed = sum(remaining_numbers) + len(numbers) - idx_number - 1
		if tiles_available >= tiles_needed:
			return calc_pure_question_marks_sequence(tiles_available, remaining_numbers)
		else:
			return 0

	count = 0
	for i in range(idx_number, len(numbers) + 1):
		numbers_to_test = numbers[idx_number:i]
		tiles_needed = sum(numbers_to_test) + i - idx_number - 1
		tiles_available = qm_blocks[idx_hashblock] - 1 - offset_from_last
		if idx_hashblock > 0 and idx_hashblock < len(hashblocks):
			tiles_available -= 1
		tolerance = tiles_available - tiles_needed
		for offset_next in range(-tolerance, 1):
			possibilities = test_hashblock(numbers, hashblocks, qm_blocks, idx_hashblock, i, offset_next)
			if possibilities > 0:
				possibilities *= calc_pure_question_marks_sequence(tiles_available + offset_next, numbers_to_test)
				count += possibilities
	return count


def test_hashblock(numbers, hashblocks, qm_blocks, idx_hashblock, idx_number, offset):
	if idx_hashblock >= len(hashblocks):
		if (idx_number >= len(numbers)):
			return 1
		else:
			return 0
		
	if idx_number >= len(numbers):
		return 0

	count = 0
	for idx_hashblock_last in range(len(hashblocks) - 1, idx_hashblock - 1, -1):
		hashblocks_stretch = hashblocks[idx_hashblock_last].endpos - hashblocks[idx_hashblock].pos
		offset_next = numbers[idx_number] - hashblocks_stretch + offset
		if offset_next >= 0:
			count += test_qm_block(numbers, hashblocks, qm_blocks, idx_hashblock_last + 1, idx_number + 1, offset_next)
	return count


def test(springs, numbers, hashblocks):
	if len(hashblocks) == 0:
		return calc_pure_question_marks_sequence(len(springs), numbers)
	
	
	qm_blocks = []
	last_qm_block_startpos = 0
	for hashblock in hashblocks:
		qm_blocks.append(hashblock.pos - last_qm_block_startpos)
		last_qm_block_startpos = hashblock.endpos
	qm_blocks.append(len(springs) - last_qm_block_startpos)

	return test_qm_block(numbers, hashblocks, qm_blocks, 0, 0, 0)


def calc_pure_question_marks_sequence(sequence_len, numbers):
	n = sequence_len - (sum(numbers) + len(numbers) - 1) + 1
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
	hashblocks = sequences[seq_index][2]
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
		sequence_numbers = numbers[num_index : i]
		possibilities_this_sequence = test_sequence(sequences, seq_index + 1, numbers, i, count)
		if possibilities_this_sequence > 0:
			possibilities_this_sequence *= test(sequence, sequence_numbers, hashblocks)
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
					# We must never reach this point because springs always ends with a '.' because
					# we appended a '.'
					raise
				pos_sequence_end += 1
			else:
				hashblocks = []
				pos = pos_sequence_start
				while pos < pos_sequence_end:
					if springs[pos] == '#':
						hashblock_start = pos
						while (pos < pos_sequence_end and springs[pos] == '#'):
							pos += 1
						hashblocks.append(Hashblock(hashblock_start - pos_sequence_start, pos - pos_sequence_start))
					pos += 1

				no_point_sequences.append((pos_sequence_end - pos_sequence_start, springs[pos_sequence_start:pos_sequence_end], hashblocks))
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
