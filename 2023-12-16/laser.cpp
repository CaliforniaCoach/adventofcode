#include <iostream>
#include <fstream>
#include <vector>

using namespace std;


#define RIGHT 0b0001
#define UP    0b0010
#define LEFT  0b0100
#define DOWN  0b1000

struct Position {
	int x;
	int y;
};

void next_pos(Position& pos, int direction) {
	switch (direction) {
		case RIGHT:
			pos.x++;
			break;

		case UP:
			pos.y--;
			break;

		case LEFT:
			pos.x--;
			break;

		case DOWN:
			pos.y++;
			break;

		default:
			throw;
	}
}

void laser_beam(const vector<string>& field, vector<vector<int>>& laserfield,
                Position pos, int direction) {

	while (true) {
		if (pos.y < 0 || pos.y >= field.size() || pos.x < 0 || pos.x >= field[pos.y].size())
			return;

		if (laserfield[pos.y][pos.x] & direction)
			return;

		laserfield[pos.y][pos.x] |= direction;

		switch (field[pos.y][pos.x]) {
			case '.':
				break;

			case '/':
				if (direction == RIGHT)
					direction = UP;
				else if (direction == UP)
					direction = RIGHT;
				else if (direction == LEFT)
					direction = DOWN;
				else if (direction == DOWN)
					direction = LEFT;
				else
					throw;
				break;

			case '\\':
				if (direction == RIGHT)
					direction = DOWN;
				else if (direction == UP)
					direction = LEFT;
				else if (direction == LEFT)
					direction = UP;
				else if (direction == DOWN)
					direction = RIGHT;
				else
					throw;
				break;

			case '|':
				if (direction == LEFT || direction == RIGHT) {
					direction = UP;
					laser_beam(field, laserfield, pos, DOWN);
				}
				break;

			case '-':
				if (direction == UP || direction == DOWN) {
					direction = RIGHT;
					laser_beam(field, laserfield, pos, LEFT);
				}
				break;

			default:
				throw;
		}
		next_pos(pos, direction);
	}
}

int count_energized_tiles(const vector<vector<int>>& laserfield) {
	int count = 0;
	for (const vector<int>& row : laserfield)
		for (int tile : row)
			if (tile != 0)
				count++;
	return count;
}

int main() {
	string line;
	vector<string> field;
	ifstream file("input.txt");
	while (getline(file, line))
		field.push_back(line);
	file.close();

	int max_energized = 0;

	for (int x = 0; x < field.size(); x++) {
		vector<vector<int>> laserfield1(field.size(), vector<int>(field[0].size(), 0));
		vector<vector<int>> laserfield2(field.size(), vector<int>(field[0].size(), 0));

		laser_beam(field, laserfield1, Position {x, 0}, DOWN);
		int energized = count_energized_tiles(laserfield1);
		if (energized > max_energized)
			max_energized = energized;

		laser_beam(field, laserfield2, Position {x, (int)field.size() - 1}, UP);
		energized = count_energized_tiles(laserfield2);
		if (energized > max_energized)
			max_energized = energized;
	}

	for (int y = 0; y < field[0].size(); y++) {
		vector<vector<int>> laserfield1(field.size(), vector<int>(field[0].size(), 0));
		vector<vector<int>> laserfield2(field.size(), vector<int>(field[0].size(), 0));

		laser_beam(field, laserfield1, Position {0, y}, RIGHT);
		int energized = count_energized_tiles(laserfield1);
		if (energized > max_energized)
			max_energized = energized;

		if (y == 0)
			cout << energized << endl;

		laser_beam(field, laserfield2, Position {(int)field[0].size() - 1, y}, LEFT);
		energized = count_energized_tiles(laserfield2);
		if (energized > max_energized)
			max_energized = energized;
	}

	cout << max_energized << endl;
}
