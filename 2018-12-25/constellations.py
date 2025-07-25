from collections import Counter


fixpoints = []

with open('input.txt', 'r') as reader:
	for i, line in enumerate(reader):
		fixpoint = tuple(map(int, line.strip().split(',')))
		entry = [fixpoint, i]
		entry.append([entry])
		fixpoints.append(entry)

for i in range(len(fixpoints) - 1):
	for k in range(i + 1, len(fixpoints)):
		if fixpoints[i][1] == fixpoints[k][1]:
			continue
		p1 = fixpoints[i][0]
		p2 = fixpoints[k][0]
		if abs(p1[0] - p2[0]) + abs(p1[1] - p2[1]) + abs(p1[2] - p2[2]) + abs(p1[3] - p2[3]) <= 3:
			constellationList1 = fixpoints[i][2]
			constellationList2 = fixpoints[k][2]
			newConstellationList = constellationList1 + constellationList2
			newConstellationId = fixpoints[i][1]
			for entry in newConstellationList:
				entry[1] = newConstellationId
				entry[2] = newConstellationList

print(fixpoints)
print(len(Counter(entry[1] for entry in fixpoints)))
print(len({entry[1] for entry in fixpoints}))

# 617 too high
