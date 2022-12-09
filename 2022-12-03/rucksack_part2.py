import sys
import string

prio_sum = 0
group_index = 0
with open(sys.argv[1], 'r') as reader:
	lines = reader.readlines()
	for group in [lines[i:i+3] for i in range(0, len(lines), 3)]:
		common = list(set(group[0][:-1]) & set(group[1][:-1]) & set(group[2][:-1]))[0]
		if common in string.ascii_lowercase:
			prio_sum += ord(common) - ord('a') + 1
		else:
			prio_sum += ord(common) - ord('A') + 26 + 1

print(prio_sum)
