import sys
import re


with open(sys.argv[1], 'r') as reader:
	content = reader.read()

words = content.split()

blueprints = {}

while words != []:
	if words[0] == 'Blueprint':
		blueprint_no = int(words[1][:-1])
		blueprints[blueprint_no] = {}
		del words[0:2]
	elif words[0] == 'Each':
		material = words[1]
		blueprints[blueprint_no][material] = {}
		del words[0:3]
		while words != [] and re.fullmatch('\\d+', words[1]):
			blueprints[blueprint_no][material][re.sub('([^\\.]+)\\.?', '\\1', words[2])] = int(words[1])
			del words[0:3]
	else:
		raise ValueError


def get_build_possibilities(build_options, robots, materials, blueprint):
	keys = build_options.keys()
	max_values = tuple(build_options.values())
	counters = [0] * len(build_options)
	if min(build_options.values()) > 0:
		# If it is possible to build all types of robots, we must build at least one,
		# otherwise we would never reach the best possible solution
		build_possiblities = []
		build_max_possible = True
	elif max(build_options.values()) > 0:
		for robot, required_materials in blueprint.items():
			if build_options[robot] == 0:
				for required_material in required_materials:
					if robots[required_material] == 0:
						break
				else:
					# It might be possible to build this robot if we just wait, therefore
					# we have to consider the possibility to build nothing now and wait
					build_possiblities = [dict(zip(keys, counters))]
					build_max_possible = False
					break
		else:
			# If we cannot build at least one of the robots by waiting long enough with the
			# currently existing robots, we must not wait and build at least something
			build_possiblities =  []
			build_max_possible = True
	else:
		build_possiblities = [dict(zip(keys, counters))]
		build_max_possible = False
	
	i = 0
	while i < len(counters):
		if counters[i] < max_values[i]:
			counters[i] += 1
			for k in range(0, i):
				counters[k] = 0
			i = 0
			build_possibility = dict(zip(keys, counters))
			new_materials = dict(materials)
			for robot, num_robots in build_possibility.items():
				for required_material, amount in blueprint[robot].items():
					new_materials[required_material] -= amount * num_robots
			if min(new_materials.values()) >= 0:
				if build_max_possible:
					for robot, possible_new_ones in build_options.items():
						if possible_new_ones > 0:
							for required_material, amount in blueprint[robot].items():
								if new_materials[required_material] < amount:
									break
							else:
								possible_to_build_one_more = True
								break
					else:
						build_possiblities.append(build_possibility)
				else:
					build_possiblities.append(build_possibility)
		else:
			i += 1

	return build_possiblities


def collect(blueprint, robots, materials, minute):
	minute += 1
	new_materials = dict(materials)
	for robot in robots:
		new_materials[robot] += robots[robot]
	if minute >= 24:
		return new_materials['geode']

	build_options = {}
	for robot in blueprint:
		can_build = -1
		for required_material, amount in blueprint[robot].items():
			if can_build < 0:
				can_build = materials[required_material] // amount
			else:
				can_build = min(can_build, materials[required_material] // amount)
		build_options[robot] = can_build

	max_geode = 0
	build_possibilities = get_build_possibilities(build_options, robots, new_materials, blueprint)
	for build_possibility in build_possibilities:
		new_robots = dict(robots)
		new_materials_after_building_robots = dict(new_materials)
		for robot in build_possibility:
			num_to_build = build_possibility[robot]
			if num_to_build > 0:
				for required_material, amount in blueprint[robot].items():
					new_materials_after_building_robots[required_material] -= amount * num_to_build
			new_robots[robot] += num_to_build
		if min(new_materials_after_building_robots.values()) >= 0:
			geode = collect(blueprint, new_robots, new_materials_after_building_robots, minute)
			max_geode = max(max_geode, geode)

	return max_geode


score = 0

for blueprint_no, blueprint in blueprints.items():
	print(f'Processing blueprint {blueprint_no}...')
	robots = dict([(robot, 0) for robot in blueprint])
	robots['ore'] = 1
	materials = dict([(robot, 0) for robot in blueprint])
	max_geode = collect(blueprint, robots, materials, 0)
	score += max_geode * blueprint_no

print(score)
