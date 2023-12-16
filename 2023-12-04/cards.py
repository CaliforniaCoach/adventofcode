total1 = 0
total2 = 0

card_stack = [1] * 500

with open("input.txt") as reader:
	for line in reader.readlines():
		nums = line.split()
		winning_numbers = nums[2:nums.index('|')]
		own_numbers = nums[nums.index('|') + 1:]
		hits = sum([1 for num in own_numbers if num in winning_numbers])
		total1 += 2 ** (hits - 1) if hits > 0 else 0

		########### Part 2 #############
		instances = card_stack[0]
		total2 += instances
		card_stack.pop(0)
		for i in range(hits):
			card_stack[i] += instances


print(f'{total1=}')
print(f'{total2=}')
