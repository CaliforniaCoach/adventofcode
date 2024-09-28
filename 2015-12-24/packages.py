import itertools
import math
from operator import itemgetter

FILENAME = 'input.txt'


def rek(valid_combinations, packets_left, packets_so_far, weight_so_far, target_total_weight, min_num_packets, first_group, num_remaining_groups):
	if first_group and len(packets_so_far) + 1 > min_num_packets:
		return valid_combinations, min_num_packets
	for packet in list(packets_left):
		packets_left.remove(packet)
		new_weight = weight_so_far + packet
		if new_weight == target_total_weight:
			packets_so_far |= {packet}
			if first_group:
				if num_remaining_groups <= 2 or find_combinations(set(packets) - packets_so_far, target_total_weight, False, num_remaining_groups - 1):
					min_num_packets = len(packets_so_far)
					return valid_combinations + [packets_so_far], min_num_packets
				else:
					break
			else:
				return valid_combinations + [packets_so_far], min_num_packets
		elif new_weight < target_total_weight:
			valid_combinations, min_num_packets = rek(valid_combinations, list(packets_left), packets_so_far | {packet}, new_weight, target_total_weight, min_num_packets, first_group, num_remaining_groups)
			if not first_group and valid_combinations:
				break
	return valid_combinations, min_num_packets

def find_combinations(packets, total_weight, first_group, num_groups):
	return rek([], sorted(list(packets), reverse=True), set(), 0, total_weight, 9999999, first_group, num_groups)

def solve(packets, num_groups):
	weight_per_group = sum(packets) // num_groups

	first_group_combinations = find_combinations(packets, weight_per_group, True, num_groups)

	solutions = [(combi, len(combi), math.prod(combi)) for combi in first_group_combinations[0]]
	solutions.sort(key=itemgetter(2))
	solutions.sort(key=itemgetter(1))

	# for solution in solutions:
	# 	print(f'LEN: {solution[1]}, QE: {solution[2]}, solution: {solution[0]}, other gropus: TBD ')
	print(f'QE: {solutions[0][2]}')


with open(FILENAME) as file:
	packets = [int(line) for line in file.readlines()]

solve(packets, 3)
solve(packets, 4)
