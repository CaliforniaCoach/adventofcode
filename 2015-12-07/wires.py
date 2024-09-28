def getval(wires, x):
	try:
		return int(x)
	except:
		return int(wires[x])

def calc(filename, interesting_wire, override_wire=None, override_value=None):
	wires = {override_wire: override_value} if override_wire else {}
	while interesting_wire not in wires:
		with open(filename) as file:
			for line in file:
				cmds = line.split()
				if not cmds:
					continue
				try:
					if cmds[1] == '->':
						if cmds[2] != override_wire:
							wires[cmds[2]] = getval(wires, cmds[0])
					elif cmds[1] == 'AND':
						wires[cmds[4]] = getval(wires, cmds[0]) & getval(wires, cmds[2])
					elif cmds[1] == 'OR':
						wires[cmds[4]] = getval(wires, cmds[0]) | getval(wires, cmds[2])
					elif cmds[1] == 'RSHIFT':
						wires[cmds[4]] = getval(wires, cmds[0]) >> getval(wires, cmds[2])
					elif cmds[1] == 'LSHIFT':
						wires[cmds[4]] = getval(wires, cmds[0]) << getval(wires, cmds[2])
					elif cmds[0] == 'NOT':
						wires[cmds[3]] = ~getval(wires, cmds[1]) & 0xffff
					else:
						raise
				except KeyError:
					pass
	return wires

a_first_run = calc('input.txt', 'a')['a']
print(a_first_run)
a_second_run = calc('input.txt', 'a', 'b', a_first_run)['a']
print(a_second_run)
