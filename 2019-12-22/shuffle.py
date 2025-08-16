DECKSIZE2 = 10007
PROCESSES = 1
TARGET_POS = 7614
# # expected result: 2019

# DECKSIZE2 = 10007
# PROCESSES = 2
# TARGET_POS = 7478
# # expected result: 2019

# DECKSIZE2 = 119315717514047
# PROCESSES = 101741582076661
# TARGET_POS = 2020

def gcdExtendedRecursive(a, b):
	if (b == 0):
		s = 1
		t = 0
		return a, s, t
	d, s1, t1 = gcdExtendedRecursive(b, a % b)
	s = t1
	t = s1 - (a // b) * t1
	return d, s, t

def pow_mod(base, exp, mod):
    result = 1
    while (True):
        if exp & 1:
            result = (result * base) % mod
        exp >>= 1
        if not exp:
            break
        base = base * base % mod
    return result

with open("input.txt") as reader:
	commands = reader.readlines()

from_pos = TARGET_POS
# pos[i] = mul[i] * pos[i - 1] + add[i]
# pos[i - 1] = mul[i - 1] * pos[i - 2] + add[i - 1]
# => pos[i] = mul[i] * (mul[i - 1] * pos[i - 2] + add[i - 1]) + add[i]
#           = (mul[i] * mul[i - 1]) * pos[i - 2] + (mul[i] * add[i - 1] + add[i])
mul = 1
add = 0
for process in range(1):
	for k in range(100 - 1, -1, -1):
		# print(from_pos)
		if commands[k].startswith("deal into new stack"):
			# from_pos = -from_pos - 1
			add_new = -1
			mul_new = -1

		elif commands[k].startswith("cut"):
			param = int(commands[k].split(" ")[-1])
			# from_pos = (from_pos + DECKSIZE2 + param) % DECKSIZE2
			# from_pos = from_pos + param
			add_new = param
			mul_new = 1

		elif commands[k].startswith("deal with increment"):
			param = int(commands[k].split(" ")[-1])
			g, s, t = gcdExtendedRecursive(param, DECKSIZE2)
			# from_pos = s * from_pos // g
			# from_pos = mod(from_pos, DECKSIZE2)
			assert g == 1
			add_new = 0
			mul_new = s

		else:
			raise

		mul = mul * mul_new % DECKSIZE2
		add = (mul_new * add + add_new) % DECKSIZE2

	from_pos = mul * from_pos + add
	print(f"After {process}st run: {from_pos}")
	from_pos %= DECKSIZE2

print(from_pos)
