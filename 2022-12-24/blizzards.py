import sys

blizzards_coords = []
blizzards_directions = []
with open(sys.argv[1], 'r') as reader:
    for y, line in enumerate(reader):
        line = line.strip()
        for x, char in enumerate(line):
            if char == '.' and (line[1] == '#' or line[2] == '#'):
                if y == 0:
                    startpos = (x, y)
                else:
                    endpos = (x, y)
            elif char in ('>', '<', 'v', '^'):
                blizzards_coords.append([x, y])
                blizzards_directions.append(char)
    width = len(line)
    height = y + 1

DIRECTIONS = {'>': (1, 0), '<': (-1, 0), 'v': (0, 1), '^': (0, -1)}


def make_way_through_blizzards(blizzards_coords, blizzards_directions, startpos, endpos):
    positions = {startpos}
    target_reached = False
    minutes = 0
    while not target_reached:
        for coords, dir in zip(blizzards_coords, blizzards_directions):
            delta = DIRECTIONS[dir]
            coords[0] = (coords[0] + delta[0] - 1) % (width - 2) + 1
            coords[1] = (coords[1] + delta[1] - 1) % (height - 2) + 1
        new_positions = set()
        for pos in positions:
            for movement in tuple(DIRECTIONS.values()) + ((0, 0),):
                x = pos[0] + movement[0]
                y = pos[1] + movement[1]
                new_pos = (x, y)
                if new_pos == endpos:
                    target_reached = True
                    break
                elif (y > 0 and y < height - 1 and x > 0 and x < width - 1 and list(new_pos) not in blizzards_coords):
                    new_positions.add(new_pos)
        new_positions.add(startpos)
        positions = new_positions
        minutes += 1

    return minutes


minutes1 = make_way_through_blizzards(blizzards_coords, blizzards_directions, startpos, endpos)
print(f'Target reached in {minutes1} minutes')
minutes2 = make_way_through_blizzards(blizzards_coords, blizzards_directions, endpos, startpos)
print(f'Made way back in {minutes2} minutes')
minutes3 = make_way_through_blizzards(blizzards_coords, blizzards_directions, startpos, endpos)
print(f'And back to the target in {minutes3} minutes')
print()
print(f'Total time: {minutes1 + minutes2 + minutes3} minutes')
