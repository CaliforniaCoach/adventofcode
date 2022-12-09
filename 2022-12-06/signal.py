import sys

num_different_chars = int(sys.argv[2])

with open(sys.argv[1], 'r') as reader:
	for line in reader:
		for i in range(num_different_chars, len(line)):
			if len(set(line[i-num_different_chars:i])) >= num_different_chars:
				print(i)
				break
