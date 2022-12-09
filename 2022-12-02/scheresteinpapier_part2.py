import sys


A = 'A'  # Stein
B = 'B'  # Papier
C = 'C'  # Schere
X = 'X'  # verlieren
Y = 'Y'  # unentschieden
Z = 'Z'  # gewinnen

PUNKTE_SPIELAUSGANG = {X: 0, Y: 3, Z: 6}
PUNKTE_SECOND = {A: 1, B: 2, C: 3}
SECOND = {
	(A, X): C, (A, Y): A, (A, Z): B, 
	(B, X): A, (B, Y): B, (B, Z): C,
	(C, X): B, (C, Y): C, (C, Z): A
}

points = 0

with open(sys.argv[1], 'r') as reader:
	for line in reader:
		first = line[0]
		second = line[2]
		points += PUNKTE_SPIELAUSGANG[second] + PUNKTE_SECOND[SECOND[(first, second)]]

print(points)
