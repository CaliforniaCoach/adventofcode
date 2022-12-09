import sys


A = 'A'
B = 'B'
C = 'C'
X = 'X'
Y = 'Y'
Z = 'Z'

PUNKTE_SHAPE = {X: 1, Y: 2, Z: 3}
PUNKTE_GEWINN = {
	(A, X): 3, (A, Y): 6, (A, Z): 0, 
	(B, X): 0, (B, Y): 3, (B, Z): 6,
	(C, X): 6, (C, Y): 0, (C, Z): 3
}

points = 0

with open(sys.argv[1], 'r') as reader:
	for line in reader:
		first = line[0]
		second = line[2]
		points += PUNKTE_SHAPE[second] + PUNKTE_GEWINN[(first, second)]

print(points)
