import sys
import re


filename = sys.argv[1]
max_minutes = int(sys.argv[2])
end_blueprint = int(sys.argv[3])
start_blueprint = int(sys.argv[4]) if len(sys.argv) >= 5 else 1


with open(filename, 'r') as reader:
	content = reader.read()

words = content.split()
blueprints = {}
max_materials_needed_per_minute = {}

while words != []:
	if words[0] == 'Blueprint':
		blueprint_no = int(words[1][:-1])
		blueprints[blueprint_no] = {}
		del words[0:2]
	elif words[0] == 'Each':
		robot = words[1]
		blueprints[blueprint_no][robot] = {}
		del words[0:3]
		while words != [] and re.fullmatch('\\d+', words[1]):
			material = re.sub('([^\\.]+)\\.?', '\\1', words[2])
			amount = int(words[1])
			blueprints[blueprint_no][robot][material] = amount
			if material in max_materials_needed_per_minute:
				max_materials_needed_per_minute[material] = max(max_materials_needed_per_minute[material], amount)
			else:
				max_materials_needed_per_minute[material] = amount
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
		# Only build new robot of this type if we do not yet produce maximum of that material needed per minute
		if robot not in max_materials_needed_per_minute or new_robots[robot] < max_materials_needed_per_minute[robot]:
			for required_material in required_materials:
				if new_robots[required_material] <= 0:
					break
			else:
				geode = collect(blueprint, new_robots, new_materials, minute, robot)
				max_geode = max(max_geode, geode)

	return max_geode


quality_level = 0
product = 1

for blueprint_no in range(start_blueprint, end_blueprint + 1):
	blueprint = blueprints[blueprint_no]
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
