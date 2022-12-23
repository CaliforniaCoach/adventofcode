import sys

sys.setrecursionlimit(100000)
monkeys = {}

with open(sys.argv[1], 'r') as reader:
	for line in reader:
		words = line.strip().split()
		if len(words) == 2:
			monkeys[words[0][:-1]] = int(words[1])
		elif len(words) == 4:
			monkeys[words[0][:-1]] = words[1:]
		else:
			raise ValueError

def get_result_from(forwards_list, backwards_list, monkey):
	value = forwards_list[monkey]
	if isinstance(value, int) or isinstance(value, float):
		return value
	elif isinstance(value, str):
		if value == 'x':
			return 'x'
		else:
			forwards_list[monkey] = get_result_from(backwards_list, forwards_list, value)
			return forwards_list[monkey]
	else:
		value1 = get_result_from(forwards_list, backwards_list, value[0])
		value2 = get_result_from(forwards_list, backwards_list, value[2])
		if isinstance(value1, int) and isinstance(value2, int):
			if value[1] == '+':
				forwards_list[monkey] = value1 + value2
			elif value[1] == '-':
				forwards_list[monkey] = value1 - value2
			elif value[1] == '*':
				forwards_list[monkey] = value1 * value2
			elif value[1] == '/':
				forwards_list[monkey] = value1 // value2
			else:
				raise ValueError
			return forwards_list[monkey]
		else:
			return 'x'

print(get_result_from(dict(monkeys), (), 'root'))

reverse = {}

def reverse_operations(monkey):
	value = monkeys[monkey]
	if isinstance(value, int):
		reverse[monkey] = value
	elif value == 'x':
		pass
	else:
		monkey1 = value[0]
		operator = value[1]
		monkey2 = value[2]
		if operator == '+':
			reverse[monkey1] = [monkey, '-', monkey2]
			reverse[monkey2] = [monkey, '-', monkey1]
		elif operator == '-':
			reverse[monkey1] = [monkey, '+', monkey2]
			reverse[monkey2] = [monkey1, '-', monkey]
		elif operator == '*':
			reverse[monkey1] = [monkey, '/', monkey2]
			reverse[monkey2] = [monkey, '/', monkey1]
		elif operator == '/':
			reverse[monkey1] = [monkey, '*', monkey2]
			reverse[monkey2] = [monkey1, '/', monkey]
		elif operator == '=':
			reverse[monkey1] = monkey2
			reverse[monkey2] = monkey1
		else:
			raise ValueError
		reverse_operations(monkey1)
		reverse_operations(monkey2)

# Replace values and operations for part 2
monkeys['root'][1] = '='
monkeys['humn'] = 'x'

# Prefill list with as many concrete result numbers as possible
get_result_from(monkeys, (), 'root')

# Determine reverse calculations and calculate value for 'humn'
reverse_operations('root')
print(get_result_from(reverse, monkeys, 'humn'))
