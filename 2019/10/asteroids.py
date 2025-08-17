import math
from fractions import Fraction
import functools

positions = []

with open("input.txt") as reader:
    for irow, row in enumerate(reader.readlines()):
        for icol, col in enumerate(row.strip()):
            if col == '#':
                positions.append((icol, irow))

def is_obstructed(x1, y1, x2, y2):
    if x1 == x2:
        stepx = 0
        stepy = math.copysign(1, y2 - y1)
    elif y1 == y2:
        stepx = math.copysign(1, x2 - x1)
        stepy = 0
    else:
        gcd = math.gcd(x2 - x1, y2 - y1)
        stepx = (x2 - x1) // gcd
        stepy = (y2 - y1) // gcd
    while True:
        x1 += stepx
        y1 += stepy
        if x1 == x2 and y1 == y2:
            return False
        elif (x1, y1) in positions:
            return True

max_visible_asteroids = 0
for a1 in positions:
    visible_asteroids = 0
    for a2 in positions:
        if a1 != a2 and not is_obstructed(a1[0], a1[1], a2[0], a2[1]):
            visible_asteroids += 1
    if visible_asteroids > max_visible_asteroids:
        max_visible_asteroids = visible_asteroids
        station = a1

print(max_visible_asteroids)

direction_map = {}

x1, y1 = station
for asteroid in positions:
    if asteroid != station:
        x2, y2 = asteroid
        # areas: 0: vertical up, 1: top right, 2: horiztontal right, 3: bottom right,
        # 4: vertical down, 5: bottom left, 6: horizontal left, 7: top left
        dx = x2 - x1
        dy = y2 - y1
        if dy < 0:
            if dx == 0:
                area = 0
            elif dx > 0:
                area = 1
            else:
                area = 7
        elif dy == 0:
            if dx > 0:
                area = 2
            elif dx < 0:
                area = 6
            else:
                raise
        else:
            if dx > 0:
                area = 3
            elif dx == 0:
                area = 4
            else:
                area = 5
        frac = Fraction(abs(dy), abs(dx)) if dx != 0 else 0
        direction_map[(area, frac)] = direction_map.get((area, frac), []) + [asteroid]

def dircomp(entry1, entry2):
    area1, frac1 = entry1
    area2, frac2 = entry2
    if area1 != area2:
        return (area1 > area2) - (area1 < area2)
    elif area1 in (3, 7):
        return (frac1 > frac2) - (frac1 < frac2)
    elif area1 in (1, 5):
        return (frac1 < frac2) - (frac1 > frac2)
    else:
        return 0

i = 0
while any(direction_map.values()):
    for direction in sorted(direction_map.keys(), key=functools.cmp_to_key(dircomp)):
        asteroids = direction_map[direction]
        if not asteroids:
            continue
        nearest = min(asteroids, key=lambda asteroid: abs(asteroid[0] - x1) + abs(asteroid[1] - y1))
        asteroids.remove(nearest)
        i += 1
        if i in (1, 2, 3, 10, 20, 50, 100, 199, 200):
            print(f'{i}th asteroid: {nearest}')
