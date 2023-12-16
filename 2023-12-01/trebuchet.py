################### Part 1 #####################

sum = 0
with open('input.txt') as reader:
	for line in reader.readlines():
		for char in line:
			if char >= '0' and char <= '9':
				sum += int(char) * 10
				break
		line = list(line)
		line.reverse()
		for char in line:
			if char >= '0' and char <= '9':
				sum += int(char)
				break
print(sum)


################### Part 2 #####################

sum = 0
NUMBERS = ('one', 'two', 'three', 'four', 'five', 'six', 'seven', 'eight', 'nine')

def is_number_word(string):
	if string[0] >= '1' and string[0] <= '9':
		return int(string[0])
	
	for i, number_word in enumerate(NUMBERS):
		if string.startswith(number_word):
			return i + 1
		
	return 0

with open('input.txt') as reader:
	for line in reader.readlines():
		for i in range(len(line)):
			num = is_number_word(line[i:])
			if num > 0:
				sum += num * 10
				break
		else:
			print('Something\'s wrong')
		for i in range(len(line) - 1, -1, -1):
			num = is_number_word(line[i:])
			if num > 0:
				sum += num
				break
		else:
			print('Something\'s wrong')

print(sum)
