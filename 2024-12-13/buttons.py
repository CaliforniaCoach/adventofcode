import sympy

def main():
	games = []
	with open("input.txt") as file:
		while True:
			line = file.readline().strip()
			if len(line) == 0:
				break
			ax, ay = line.split(": ")[1].split(", ")
			bx, by = file.readline().strip().split(": ")[1].split(", ")
			px, py = file.readline().strip().split(": ")[1].split(", ")
			file.readline()
			
			games.append((int(ax[2:]), int(ay[2:]), int(bx[2:]), int(by[2:]), int(px[2:]), int(py[2:])))

		
	print(f"Tokens needed to win all possible prizes: {calc_required_tokens(games, 0)}")
	print(f"Tokens needed to win all possible prizes with offset: {calc_required_tokens(games, 10000000000000)}")


def calc_required_tokens(games, p_offset):
	a, b = sympy.symbols("a b", integer=True)
	tokens = 0
	for (ax, ay, bx, by, px, py) in games:
		solutions = sympy.linsolve((a * ax + b * bx - (px + p_offset), a * ay + b * by - (py + p_offset)), (a, b))
		if len(solutions) != 1:
			print(f"Warning: Multiple solutions found for game {ax=}, {ay=}, {bx=}, {by=}, {px=}, {py=}")
		slnA, slnB = next(iter(solutions))
		if slnA.is_nonnegative and slnB.is_nonnegative and slnA.is_Integer and slnB.is_Integer:
			tokens += 3 * slnA + slnB
	return tokens


if __name__ == "__main__":
	main()
