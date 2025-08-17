import itertools
import resource
import sys


def get_index(complist, comp):
	if comp in complist:
		return complist.index(comp)
	else:
		complist.append(comp)
		return len(complist) - 1


connections =  []
comps = {}
complist = []

with open("input.txt") as reader:
	for line in reader.readlines():
		wiring = line.split(':')
		comp1 = wiring[0]
		comp2 = wiring[1] .split()
		for comp in comp2:
			comps[get_index(complist, comp1)] = comps.setdefault(get_index(complist, comp1), set()) | {get_index(complist, comp)}
			comps[get_index(complist, comp)] = comps.setdefault(get_index(complist, comp), set()) | {get_index(complist, comp1)}
			# connections.append[(comp, comp1)]
			# connections[comp] = comp1

resource.setrlimit(resource.RLIMIT_STACK, (2**29, -1))
sys.setrecursionlimit(10**6)


def step(comps, comp, target_comp, path_so_far, max_len):
	path_so_far = path_so_far + [comp]

	if comp == target_comp:
		return path_so_far
	
	if len(path_so_far) >= max_len:
		return None

	if target_comp in comps[comp]:
		return path_so_far + [target_comp]
	
	for next_comp in comps[comp]:
		shortest_path = step(comps, next_comp, target_comp, path_so_far, max_len)
		if shortest_path:
			return shortest_path

	return None


def find_shortest_path(comps, start_comp, target_comp, max_len):
	return step(comps, start_comp, target_comp, [], max_len + 1)


def copy(comps):
	new_comp = {}
	for key, val in comps.items():
		new_comp[key] = set(val)
	return new_comp


def fill(comps, comp, num, target, path, paths):
	path.append(comp)
	if comp == target:
		paths.append(list(path))
	if comp in comps:
		num += 1
		next_comps = comps[comp]
		del comps[comp]
		for next_comp in next_comps:
			num = fill(comps, next_comp, num, target, path, paths)
	path.pop()
	return num


def count_reachable_comps(distmap):
	count = 0
	for entry in distmap:
		if entry < 9999999:
			count += 1
	return count


def all_paths(comps, distmap, comp, target_comp, max_num_paths):
	comps = copy(comps)
	paths = []
	print('Evaluating', comp, target_comp)
	while len(paths) <= max_num_paths:
		if distmap[target_comp] < 9999999:
			shortest_path = find_shortest_path(comps, comp, target_comp, distmap[target_comp])
			paths.append(shortest_path)
			for i in range(len(shortest_path) - 1):
				comps[shortest_path[i]] -= {shortest_path[i + 1]}
				comps[shortest_path[i + 1]] -= {shortest_path[i]}
			distmap = create_distmap(comps, comp)
		else:
			break

	groupsize1 = count_reachable_comps(distmap)
	groupsize2 = fill(comps, target_comp, 0, '', [], [])
	return paths, groupsize1, groupsize2


def distmap_step(comps, distmap, comp, cur_dist):
	if distmap[comp] <= cur_dist:
		return
	
	distmap[comp] = cur_dist
	for next_comp in comps[comp]:
		distmap_step(comps, distmap, next_comp, cur_dist + 1)


def create_distmap(comps, start_comp):
	distmap = [9999999] * len(comps)
	distmap_step(comps, distmap, start_comp, 0)
	return distmap


fewest_paths = None
comps2 = copy(comps)
for comp1 in comps:
	del comps2[comp1]
	distmap = create_distmap(comps, comp1)
	for comp2 in comps2:
		paths, size1, size2 = all_paths(comps, distmap, comp1, comp2, len(fewest_paths) if fewest_paths else 3)
		if not fewest_paths or len(paths) < len(fewest_paths):
			fewest_paths = paths
			solution1 = size1
			solution2 = size2
			print(f'   Intermediate solution with {len(fewest_paths)} disconnected wires: {fewest_paths}')
			print(f'   Group sizes: {solution1} * {solution2} = {solution1 * solution2}')

print()
print('Final solution:')
print(f'{len(fewest_paths)} disconnected wires')
print(fewest_paths)
print(f'Group sizes: {solution1} * {solution2} = {solution1 * solution2}')
