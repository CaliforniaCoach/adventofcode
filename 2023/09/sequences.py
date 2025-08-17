sequences = []

with open('input.txt') as reader:
	for line in reader.readlines():
		sequences.append([[int(x) for x in line.split()]])

sum = 0
sumprev = 0
for sequence in sequences:
	while max(sequence[-1]) != 0 or min(sequence[-1]) != 0:
		sequence.append([sequence[-1][i + 1] - sequence[-1][i] for i in range(len(sequence[-1]) - 1)])
	x = 0
	xprev = 0
	for series in reversed(sequence):
		x += series[-1]
		series.append(x)
		xprev = series[0] - xprev
		series.insert(0, xprev)
	sum += x
	sumprev += xprev

print(sum, sumprev)
