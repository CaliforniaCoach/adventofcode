import sys
import re


startpoints = sys.argv[2]

with open(sys.argv[1], 'r') as reader:
    grid = [line.strip() for line in reader.readlines()]

visited = [['.'] * len(line) for line in grid]
min_steps_to_here = [[9999999] * len(line) for line in grid]

min_length = 9999999

def continue_from_field(x, y, last_height, steps):
    global min_length
    global min_steps_to_here
    if grid[y][x] == 'E':
        if last_height >= 'z':
            min_length = min(min_length, steps)
            for line in visited:
                for x in line:
                    print(x, end='')
                print()
            print()
    elif steps < min_steps_to_here[y][x]:
        min_steps_to_here[y][x] = steps
        for dir in ((0, 1, '>'), (0, -1, '<'), (1, 0, 'v'), (-1, 0, '^')):
            visited[y][x] = dir[2]
            next_x = x + dir[1]
            next_y = y + dir[0]
            if (next_y >= 0 and next_y < len(grid) and next_x >= 0 and next_x < len(grid[next_y]) 
                    and visited[next_y][next_x] == '.' and grid[next_y][next_x] not in startpoints
                    and (ord(grid[next_y][next_x]) - ord(grid[y][x]) <= 1 or grid[y][x] == 'S')):
                continue_from_field(next_x, next_y, grid[y][x], steps + 1)
        visited[y][x] = '.'

for y, line in enumerate(grid):
    for x, field in enumerate(line):
        if field in startpoints:
            # grid[y] = re.sub('S', 'a', grid[y])
            continue_from_field(x, y, 'a', 0)

print(min_length)
