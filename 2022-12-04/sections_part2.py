import sys

def inside(x, start, end):
	return x >= start and x <= end

counter = 0
with open(sys.argv[1], 'r') as reader:
	for line in reader:
		elves = line.split(',')

		elve0 = elves[0].split('-')
		elve1 = elves[1].split('-')
		
		elve0start = int(elve0[0])
		elve0end = int(elve0[1])
		elve1start = int(elve1[0])
		elve1end = int(elve1[1])
		
		if (inside(elve0end, elve1start, elve1end)
			or inside(elve0end, elve1start, elve1end)
			or inside(elve1start, elve0start, elve0end)
			or inside(elve1end, elve0start, elve0end)
		):
			counter += 1

print(counter)
