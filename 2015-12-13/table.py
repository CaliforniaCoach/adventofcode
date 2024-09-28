import itertools

happiness = {}

with open('input.txt') as file:
	for line in file:
		tok = line.split()
		val = int(tok[3]) if tok[2] == 'gain' else -int(tok[3])
		if tok[0] not in happiness:
			happiness[tok[0]] = {tok[10][:-1]: val}
		else:
			happiness[tok[0]][tok[10][:-1]] = val

def maxgain(happiness):
	maxgain = 0
	for arrangement in itertools.permutations(happiness.keys()):
		gain = 0
		for i in range(len(arrangement)):
			gain += happiness[arrangement[i]][arrangement[(i + 1) % len(arrangement)]]
			gain += happiness[arrangement[(i + 1) % len(arrangement)]][arrangement[i]]
		maxgain = max(maxgain, gain)
	return maxgain

print(maxgain(happiness))

happiness['myself'] = {person: 0 for person in happiness.keys()}
for entry in happiness.values():
	entry['myself'] = 0

print(maxgain(happiness))
