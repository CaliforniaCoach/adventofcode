import sys
import string

prio_sum = 0
with open(sys.argv[1], 'r') as reader:
	for line in reader:
		compartment1 = set(line[: int(len(line) / 2)])
		compartment2 = set(line[int(len(line) / 2) :])
		common = list(compartment1 & compartment2)[0]
		if common[0] in string.ascii_lowercase:
			prio_sum += ord(common) - ord('a') + 1
		else:
			prio_sum += ord(common) - ord('A') + 26 + 1

print(prio_sum)
