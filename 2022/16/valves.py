import sys
import re
import itertools


valves = {}
active_valves = {}
navi_map = {}
total_active_valves = 0


def navigate_from(valve, target_valve, path, hops, min_path, min_distance):
	if valve == target_valve:
		return path, hops
	elif hops >= min_distance:
		return min_path, min_distance
	else:
		for next_tunnel in valves[valve][1]:
			if next_tunnel not in path:
				returned_path, distance = navigate_from(next_tunnel, target_valve, path + (next_tunnel, ), hops + 1, min_path, min_distance)
				if distance < min_distance:
					min_distance = distance
					min_path = returned_path
				if hops >= min_distance:
					break
		return min_path, min_distance


def calc_navi_map(start_valve):
	global navi_map
	for valve1, valve2 in itertools.combinations(tuple(active_valves) + (start_valve, ), 2):
		path, distance = navigate_from(valve1, valve2, (), 0, (), 9999999)
		navi_map[(valve1, valve2)] = path
		navi_map[(valve2, valve1)] = tuple(reversed(path[:-1])) + (valve1, )


def continue_from(valve_name, minutes_elapsed, pressure_released, total_flow_rate, on_path, valves_still_closed):
	global valves, max_pressure_released, max_moves

	# Update values
	minutes_elapsed += 1
	pressure_released += total_flow_rate
	if minutes_elapsed >= max_moves:
		max_pressure_released = max(max_pressure_released, pressure_released)
		return

	if valves_still_closed == set():
		# Just wait, do nothing anymore. Stay here, wait for released pressure to rise
		continue_from(valve_name, minutes_elapsed, pressure_released, total_flow_rate, (), set())
		return

	valve = valves[valve_name]
	if on_path == ():
		if valve[0] > 0 and valve_name in valves_still_closed:
			# We have reached the end of our path and open the valve now
			continue_from(valve_name, minutes_elapsed, pressure_released, total_flow_rate + valve[0], (), valves_still_closed - {valve_name})
		else:
			# We have opened the current valve (or are at the start valve); now continue to other valves which are still closed
			for next_target in valves_still_closed:
				if next_target != valve_name:
					path = navi_map[(valve_name, next_target)]
					continue_from(path[0], minutes_elapsed, pressure_released, total_flow_rate, path[1:], valves_still_closed)

	else:
		# If we are on a path to a target already, continue following it to its end
		continue_from(on_path[0], minutes_elapsed, pressure_released, total_flow_rate, on_path[1:], valves_still_closed)


def continue_from_with_elephant(current_positions, minutes_elapsed, pressure_released, total_flow_rate, on_paths, valves_still_closed, valves_to_open):
	global valves, max_pressure_released

	# Update values
	minutes_elapsed += 1
	pressure_released += total_flow_rate
	if minutes_elapsed >= max_moves:
		max_pressure_released = max(max_pressure_released, pressure_released)
		return

	if valves_still_closed == set():
		# Just wait, do nothing anymore. Stay here, wait for released pressure to rise
		continue_from_with_elephant(current_positions, minutes_elapsed, pressure_released, total_flow_rate, on_paths, set(), set())
		return

	new_positions = []
	new_paths = []
	new_targets_required_from = []
	for current_position, on_path in zip(current_positions, on_paths):
		valve = valves[current_position]
		if on_path == ():
			if valve[0] > 0 and current_position in valves_still_closed:
				# We have reached the end of our path and open the closed valve now
				new_positions.append(current_position)
				new_paths.append(())
				valves_still_closed = valves_still_closed - {current_position}
				total_flow_rate += valve[0]
			else:
				# We have opened the current valve (or are at the start valve); now continue to other valves which are still closed
				new_targets_required_from.append(current_position)
		else:
			# If we are on a path to a target already, continue following it to its end
			new_positions.append(on_path[0])
			new_paths.append(on_path[1:])

	if new_targets_required_from == []:
		continue_from_with_elephant(new_positions, minutes_elapsed, pressure_released, total_flow_rate, new_paths, valves_still_closed, valves_to_open)
	else:
		no_targets_required = max(len(new_targets_required_from) - len(valves_to_open), 0)
		for new_targets in itertools.permutations(tuple(valves_to_open) + (None, ) * no_targets_required, len(new_targets_required_from)):
			new_positions2 = []
			new_paths2 = []
			for current_position, new_target in zip(new_targets_required_from, new_targets):
				if new_target != None:
					path = navi_map[(current_position, new_target)]
					new_positions2.append(path[0])
					new_paths2.append(path[1:])
				else:
					new_positions2.append(current_position)
					new_paths2.append(())
			continue_from_with_elephant(new_positions + new_positions2, minutes_elapsed, pressure_released, total_flow_rate, new_paths + new_paths2, valves_still_closed, valves_to_open - set(new_targets))


def start_with_elephant(start_pos, num_workers):
	active_valves_set = set(active_valves)
	for new_targets in itertools.combinations(active_valves, num_workers):
		new_positions = []
		new_paths = []
		for current_position, new_target in zip((start_pos, ) * num_workers, new_targets):
			path = navi_map[(current_position, new_target)]
			new_positions.append(path[0])
			new_paths.append(path[1:])
		continue_from_with_elephant(new_positions, 1, 0, 0, new_paths, active_valves_set, active_valves_set - set(new_targets))


with open(sys.argv[1], 'r') as reader:
	for line in reader:
		matcher = re.fullmatch('Valve ([A-Z]+) has flow rate=(\\d+); tunnels? leads? to valves? ([A-Z ,]+)', line.strip())
		valve = matcher.group(1)
		flowrate = int(matcher.group(2))
		tunnels = matcher.group(3).split(', ')
		valves[valve] = [flowrate, tunnels, False]
		if flowrate > 0:
			active_valves[valve] = valves[valve]
			total_active_valves += 1

calc_navi_map('AA')

max_moves = 30
max_pressure_released = 0
continue_from('AA', 0, 0, 0, (), set(active_valves))
print(max_pressure_released)

max_moves = 30
max_pressure_released = 0
# continue_from_with_elephant(('AA', ), 0, 0, 0, ((), ()), set(active_valves), set(active_valves))
start_with_elephant('AA', 1)
print(max_pressure_released)

max_moves = 26
max_pressure_released = 0
# continue_from_with_elephant(('AA', 'AA'), 0, 0, 0, ((), ()), set(active_valves), set(active_valves))
start_with_elephant('AA', 2)
print(max_pressure_released)
