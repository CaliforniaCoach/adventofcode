import sys

with open(sys.argv[1], 'r') as reader:
	trees = [x.strip() for x in reader.readlines()]

print(trees)

visible_count = 2 * len(trees) + len(trees[0]) + len(trees[-1]) - 4
scenic_score_max = 0

print(visible_count)
for y in range(1, len(trees) - 1):
	for x in range(1, len(trees[y]) - 1):
		visible = False
		scenic_score = 1

		scenic_one_direction = 0
		for xvis in range(x + 1, len(trees[y])):
			scenic_one_direction += 1
			if int(trees[y][x]) <= int(trees[y][xvis]):
				break  # out of "for xvis" loop
		else:
			visible = True
		scenic_score *= scenic_one_direction

		scenic_one_direction = 0
		for xvis in range(x - 1, -1, -1):
			scenic_one_direction += 1
			if int(trees[y][x]) <= int(trees[y][xvis]):
				break  # out of "for xvis" loop
		else:
			visible = True
		scenic_score *= scenic_one_direction

		scenic_one_direction = 0
		for yvis in range(y + 1, len(trees)):
			scenic_one_direction += 1
			if int(trees[y][x]) <= int(trees[yvis][x]):
				break  # out of "for yvis" loop
		else:
			visible = True
		scenic_score *= scenic_one_direction

		scenic_one_direction = 0
		for yvis in range(y - 1, -1, -1):
			scenic_one_direction += 1
			if int(trees[y][x]) <= int(trees[yvis][x]):
				break  # out of "for yvis" loop
		else:
			visible = True
		scenic_score *= scenic_one_direction
		scenic_score_max = max(scenic_score_max, scenic_score)
		if visible:
			visible_count += 1
			

print(f'{visible_count=}')
print(f'{scenic_score_max=}')
