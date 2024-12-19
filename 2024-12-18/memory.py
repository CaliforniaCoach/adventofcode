import sys

sys.setrecursionlimit(100000)

def print_mem(mem, path=None):
    for y, row in enumerate(mem):
        for x, tile in enumerate(row):
            if tile < 0:
                print("#", end="")
            elif path and (x, y) in path:
                print("O", end="")
            else:
                print(".", end="")
        print()
    print()

def move_to_next(mem, x, y, steps_so_far):
    if x < 0 or y < 0 or x >= len(mem[0]) or y >= len(mem):
        return None
    if mem[y][x] < 0 or steps_so_far >= mem[y][x]:
        return None
    mem[y][x] = steps_so_far
    if x == 70 and y == 70:
        return [(x, y)]
    shortest_subseq_path = None
    for (dx, dy) in [(1, 0), (-1, 0), (0, 1), (0, -1)]:
        subseq_path = move_to_next(mem, x + dx, y + dy, steps_so_far + 1)
        if subseq_path is not None:
            if shortest_subseq_path is None or len(subseq_path) < len(shortest_subseq_path):
                shortest_subseq_path = subseq_path
    return shortest_subseq_path + [(x, y)] if shortest_subseq_path else None

def find_shortest_path(mem):
    return move_to_next([list(row) for row in mem], 0, 0, 0)


locations = []
with open("input.txt", encoding="ascii") as file:
    for line in file.readlines():
        tok = line.split(",")
        locations.append((int(tok[0]), int(tok[1])))

memory = [[99999999] * 71 for _ in range(71)]
for i in range(1024):
    loc = locations[i]
    memory[loc[1]][loc[0]] = -1

shortest_path = find_shortest_path(memory)
print(f"Min number of steps to reach exit after first 1024 bytes: {len(shortest_path) - 1}")
print_mem(memory, shortest_path)

for i in range(1024, len(locations)):
    loc = locations[i]
    memory[loc[1]][loc[0]] = -1
    if loc in shortest_path:
        shortest_path = find_shortest_path(memory)
        print_mem(memory, shortest_path)
        if not shortest_path:
            print(f"Exit not reachable anymore after adding byte with location {loc[0]},{loc[1]}")
            break
