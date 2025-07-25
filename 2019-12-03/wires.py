wires = []
with open("input.txt") as reader:
    for line in reader.readlines():
        wires.append([(x[0], int(x[1:])) for x in line.strip().split(",")])

wire_parts = []
for wire in wires:
    # horizontals: list of tuples: (y_coord, (x_coord_start, x_coord_end),
    #                               total_steps_to_endpoint, left_to_right)
    horizontals = []
    # verticals: list of tuples: (x_coord, (y_coord_start, y_coord_end),
    #                             total_steps_to_endpoint, bottom_to_top)
    verticals = []
    x = 0
    y = 0
    total_steps = 0
    for dir, steps in wire:
        if dir == "U":
            verticals.append((x, (y, y + steps), total_steps, True))
            y += steps
        elif dir == "D":
            verticals.append((x, (y - steps, y), total_steps, False))
            y -= steps
        elif dir == "R":
            horizontals.append((y, (x, x + steps), total_steps, True))
            x += steps
        elif dir == "L":
            horizontals.append((y, (x - steps, x), total_steps, False))
            x -= steps
        else:
            raise
        total_steps += steps
    wire_parts.append((horizontals, verticals))

def get_crossings(horizontals, verticals):
    crossings = []
    for p1 in horizontals:
        for p2 in verticals:
            if p1[0] >= p2[1][0] and p1[0] <= p2[1][1] and p2[0] >= p1[1][0] and p2[0] <= p1[1][1]:
                if p1[3]:
                    steps1 = p1[2] + p2[0] - p1[1][0]
                else:
                    steps1 = p1[2] + p1[1][1] - p2[0]
                if p2[3]:
                    steps2 = p2[2] + p1[0] - p2[1][0]
                else:
                    steps2 = p2[2] + p2[1][1] - p1[0]
                crossings.append((p2[0], p1[0], steps1 + steps2))
    return crossings

# crossings: List of tuples: (x_coord, y_coord, sum_of_num_steps_of_both_wires_from_starting_point)
crossings = []
for i1, wire1 in enumerate(wire_parts):
    for wire2 in wire_parts[i1 + 1:]:
        crossings.extend(get_crossings(wire1[0], wire2[1]))
        crossings.extend(get_crossings(wire2[0], wire1[1]))

print(min([abs(x) + abs(y) for x, y, steps in crossings if (x, y) != (0, 0)]))
print(min([steps for x, y, steps in crossings if (x, y) != (0, 0)]))
