import sys
import re


filename = sys.argv[1]
max_minutes = int(sys.argv[2])
max_blueprints = int(sys.argv[3])

with open(filename, 'r') as reader:
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


def collect(blueprint, robots, materials, minute, next_to_build):
	minute += 1
	new_materials = dict(materials)
	new_robots = dict(robots)
	for robot in robots:
		new_materials[robot] += robots[robot]
	if minute >= max_minutes:
		return new_materials['geode']

	# If robot can be built: build it
	if next_to_build:
		materials_after_build = dict(new_materials)
		for required_material, amount in blueprint[next_to_build].items():
			# Compare against materials available at beginning of the minute
			if materials[required_material] < amount:
				return collect(blueprint, new_robots, new_materials, minute, next_to_build)
			else:
				materials_after_build[required_material] -= amount
		else:
			# Build it!
			new_robots[next_to_build] += 1
			new_materials = materials_after_build

	# Determine which robot types can be built next when waiting long enough with current robots
	max_geode = 0
	for robot, required_materials in blueprint.items():
		for required_material in required_materials:
			if new_robots[required_material] <= 0:
				break
		else:
			geode = collect(blueprint, new_robots, new_materials, minute, robot)
			max_geode = max(max_geode, geode)

	return max_geode


def collect_method_2(blueprint, robots, materials, minute, next_to_build):
	new_materials = dict(materials)
	new_robots = dict(robots)

	if next_to_build:
		required_minutes = 1
		for required_material, amount in blueprint[next_to_build].items():
			# Compare against materials available at beginning of the minute
			required_minutes = max(required_minutes, (amount - materials[required_material] - 1) // robots[required_material] + 2)

		if minute + required_minutes < max_minutes:
			new_robots[next_to_build] += 1
			for required_material, amount in blueprint[next_to_build].items():
				new_materials[required_material] -= amount
		else:
			required_minutes = max_minutes - minute
		
		minute += required_minutes
		for robot in robots:
			new_materials[robot] += robots[robot] * required_minutes

	if minute >= max_minutes:
		return new_materials['geode']

	# Determine which robot types can be built next when waiting long enough with current robots
	max_geode = 0
	for robot, required_materials in blueprint.items():
		for required_material in required_materials:
			if new_robots[required_material] <= 0:
				break
		else:
			geode = collect(blueprint, new_robots, new_materials, minute, robot)
			max_geode = max(max_geode, geode)

	return max_geode




quality_level = 0
product = 1

for i, (blueprint_no, blueprint) in enumerate(blueprints.items()):
	if i >= max_blueprints:
		break
	print(f'Processing blueprint {blueprint_no}...')
	robots = dict([(robot, 0) for robot in blueprint])
	robots['ore'] = 1
	materials = dict([(robot, 0) for robot in blueprint])
	max_geode = collect(blueprint, robots, materials, 0, None)
	print(f'    ... {max_geode=}')
	quality_level += max_geode * blueprint_no
	product *= max_geode

print()
print(f'{quality_level=}, {product=}')
