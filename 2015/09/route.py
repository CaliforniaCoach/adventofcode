import itertools

distmap = {}

with open('input.txt') as file:
	for line in file:
		tok = line.split()
		if tok[0] in distmap:
			distmap[tok[0]][tok[2]] = int(tok[4])
		else:
			distmap[tok[0]] = {tok[2]: int(tok[4])}
		if tok[2] in distmap:
			distmap[tok[2]][tok[0]] = int(tok[4])
		else:
			distmap[tok[2]] = {tok[0]: int(tok[4])}

mindist = 9999999999999
maxdist = 0

for route in itertools.permutations(distmap.keys()):
	dist = 0
	for i in range(len(route) - 1):
		dist += distmap[route[i]][route[i + 1]]
	mindist = min(mindist, dist)
	maxdist = max(maxdist, dist)

print(mindist, maxdist)
