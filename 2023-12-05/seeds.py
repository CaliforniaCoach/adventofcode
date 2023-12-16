maps = []
current_map = []

with open("input.txt") as reader:
	seeds = [int(seed) for seed in reader.readline().split()[1:]]

	for line in reader.readlines():
		line = line.strip()
		if len(line) != 0:
			if line[0] >= '0' and line [0] <= '9':
				current_map.append([int(num) for num in line.split()])
			else:
				maps.append(current_map)
				current_map = []
	maps.append(current_map)
	maps.pop(0)

codes = seeds
for map in maps:
	new_codes = []
	for code in codes:
		for dest, src, length in map:
			if code >= src and code < src + length:
				new_codes.append(dest + code - src)
				break
		else:
			new_codes.append(code)
	codes = new_codes

print(min(codes))

######################## Part 2 ##########################

ranges = [(seeds[i], seeds[i + 1]) for i in range(0, len(seeds), 2)]

for map in maps:
	map.sort(key=lambda x: x[1])
	for i in range(len(map) - 1):
		if map[i][1] + map[i][2] < map[i + 1][1]:
			map.insert(i + 1, [map[i][1] + map[i][2], map[i][1] + map[i][2], map[i + 1][1] - (map[i][1] + map[i][2])])
	new_ranges = []
	for startcode, codelen in ranges:
		if startcode < map[0][1]:
			new_ranges.append((startcode, map[0][1] - startcode))
		for dest, src, length in map:
			if startcode < src + length and startcode + codelen >= src:
				if startcode < src + length:
					if startcode + codelen > src + length:
						new_ranges.append((dest + max(startcode, src) - src, min(length, src + length - startcode)))
					else:
						new_ranges.append((dest + max(startcode, src) - src, startcode + codelen - max(startcode, src)))
		if startcode + codelen > map[-1][1] + map[-1][2]:
			new_ranges.append((max(startcode, (map[-1][1] + map[-1][2])), min(startcode + codelen - (map[-1][1] + map[-1][2]), codelen)))
	ranges = new_ranges

print(min([entry[0] for entry in ranges]))
