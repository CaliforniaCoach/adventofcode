import sys
import re


sensors = []

def odd(x):
    return x % 2 != 0


def even(x):
    return x % 2 == 0


def merge_ranges(old_range, from_x, to_x):
    from_x_inside_a_range = False
    from_x_outside_of_ranges = False
    from_x_insert_index = -1
    new_range = []
    for i, range in enumerate(old_range):
        if not from_x_inside_a_range and not from_x_outside_of_ranges:
            if from_x > range + 1:
                new_range.append(range)
            else:
                if odd(i):
                    from_x_inside_a_range = True
                else:
                    from_x_outside_of_ranges = True
                    new_range.append(from_x)
        if from_x_inside_a_range or from_x_outside_of_ranges:
            if to_x < range:
                if even(i):
                    new_range.append(to_x)
                    new_range += old_range[i:]
                else:
                    new_range.append(range)
                    new_range += old_range[i + 1:]
                break
    else:
        if not from_x_inside_a_range and not from_x_outside_of_ranges:
            new_range.append(from_x)
        new_range.append(to_x)
    
    return new_range


def calc_covered_ranges_for_row(sensors, relevant_y):
    covered_ranges = []
    beacons_in_row = set()

    for sensor in sensors:
        sensor_x = sensor[0]
        sensor_y = sensor[1]
        beacon_x = sensor[2]
        beacon_y = sensor[3]
        dist = sensor[4]

        if beacon_y == relevant_y:
            beacons_in_row.add(beacon_x)

        if abs(relevant_y - sensor_y) <= dist:
            width = 2 * (dist - abs(sensor_y - relevant_y)) + 1
            from_x = sensor_x - width // 2
            to_x = sensor_x + width // 2
            covered_ranges = merge_ranges(covered_ranges, from_x, to_x)

    num_covered = sum(covered_ranges[1::2]) - sum(covered_ranges[::2]) + len(covered_ranges) // 2
    for beacon_x in beacons_in_row:
        for i in range(len(covered_ranges)):
            if beacon_x >= covered_ranges[i] and beacon_y <= covered_ranges[i + 1]:
                num_covered -= 1
                break

    return covered_ranges, num_covered


relevant_y = int(sys.argv[2])
with open(sys.argv[1], 'r') as reader:
    for line in reader:
        line = line.strip()
        matcher = re.fullmatch('Sensor at x=([\\d-]+), y=([\\d-]+): closest beacon is at x=([\\d-]+), y=([\\d-]+)', line)
        sensor_x = int(matcher.group(1))
        sensor_y = int(matcher.group(2))
        beacon_x = int(matcher.group(3))
        beacon_y = int(matcher.group(4))
        dist = abs(sensor_x - beacon_x) + abs(sensor_y - beacon_y)
        sensors.append((sensor_x, sensor_y, beacon_x, beacon_y, dist))

covered_ranges, num_covered = calc_covered_ranges_for_row(sensors, relevant_y)

print(f'Row {relevant_y}: {covered_ranges=}, {num_covered=}')

max_x_y = int(sys.argv[3])

for y in range(max_x_y + 1):
    covered_ranges, num_covered = calc_covered_ranges_for_row(sensors, y)
    if len(covered_ranges) > 2 or num_covered < max_x_y:
        print(f'Row {y}: {covered_ranges=}, {num_covered=}')
        for i in range(1, len(covered_ranges) - 1, 2):
            for x in range(covered_ranges[i] + 1, covered_ranges[i+1]):
                print(f'    Missing: x/y = {x}/{y}, frequency {x * max_x_y + y}')
