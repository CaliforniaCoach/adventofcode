#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>

using namespace std;


#define FILENAME "input.txt"

#define MINIMAL_SOLUTION
// #define ALL_SOLUTIONS


#ifdef MINIMAL_SOLUTION
	#ifdef ALL_SOLUTIONS
		#error Exactly one of "MINIMAL_SOLUTION" or "ALL_SOLUTIONS" must be defined
	#endif
	#define COMPARISON_OP >=
#else
	#ifdef ALL_SOLUTIONS
		#define COMPARISON_OP >
	#else
		#error Either "MINIMAL_SOLUTION" or "ALL_SOLUTIONS" must be defined
	#endif
#endif

struct Position {
	int x;
	int y;
};

struct MinHeatloss {
	int val = 9999999;
	int steps = 3;
};

#define RIGHT 0
#define UP    1
#define LEFT  2
#define DOWN  3


ostream& operator<<(ostream& os, const vector<int>& vec) {
	for (int x : vec)
		os << setw(3) << x << " ";
	return os;
}

class Heatfield {

	public:

	vector<vector<int>> field;
	vector<vector<vector<MinHeatloss>>> min_heatloss;
	int size_x;
	int size_y;
	int min_total_heatloss;
	vector<int> path;
	vector<vector<int>> best_paths;
	vector<int> heatloss_along_path;
	vector<vector<int>> best_heatloss_paths;

	Heatfield(bool ultra_crucible = false) {
		string line;
		ifstream file(FILENAME);
		while (getline(file, line)) {
			vector<int> row;
			vector<vector<MinHeatloss>> min_heatloss_row;
			for (char c : line) {
				row.push_back(c - '0');
				min_heatloss_row.push_back(vector<MinHeatloss>(4));
			}
			field.push_back(row);
			min_heatloss.push_back(min_heatloss_row);
		}
		file.close();

		size_x = field[0].size();
		size_y = field.size();

		min_total_heatloss = 99999;
		int heatloss = -field[0][0];
		go_step({0, 0}, RIGHT, 0, heatloss, ultra_crucible);

		cout << min_total_heatloss << ": " << endl;
		for (int i = 0; i < best_paths.size(); i++)
			cout << best_paths[i] << endl << best_heatloss_paths[i] << endl << endl;
	}

	void go_step(Position pos, int direction, int num_steps_in_direction, int heatloss,
	             bool ultra_crucible = false) {

		if (pos.x < 0 || pos.x >= size_x || pos.y < 0 || pos.y >= size_y)
			return;
		if (ultra_crucible && num_steps_in_direction < 4) {
			switch (direction) {
				case RIGHT:
					if (size_x - 1 - pos.x < 4 - num_steps_in_direction)
						return;
					break;
				case DOWN:
					if (size_y - 1 - pos.y < 4 - num_steps_in_direction)
						return;
					break;
				case LEFT:
					if (pos.x < 4 - num_steps_in_direction)
						return;
					break;
				case UP:
					if (pos.y < 4 - num_steps_in_direction)
						return;
					break;
			}
		}

		heatloss += field[pos.y][pos.x];
		int min_remaining_heatloss = (size_x - 1 - pos.x) + (size_y - 1 - pos.y);
		if (heatloss + min_remaining_heatloss COMPARISON_OP min_total_heatloss)
			return;

		if (pos.x == size_x - 1 && pos.y == size_y - 1) {
			if (heatloss < min_total_heatloss) {
				best_paths.clear();
				best_heatloss_paths.clear();
			}
			min_total_heatloss = heatloss;

			vector<int> best_path(path.begin() + 1, path.end());
			best_path.push_back(direction);
			best_paths.push_back(best_path);

			vector<int> best_hl_path(heatloss_along_path.begin() + 1, heatloss_along_path.end());
			best_hl_path.push_back(heatloss);
			best_heatloss_paths.push_back(best_hl_path);
			return;
		}

		int next_dir1, next_dir2;
		Position next_pos1, next_pos2;
		switch (direction) {
			case RIGHT:
			case LEFT:
				next_dir1 = DOWN;
				next_pos1 = {pos.x, pos.y + 1};
				next_dir2 = UP;
				next_pos2 = {pos.x, pos.y - 1};
				break;

			case UP:
			case DOWN:
				next_dir1 = RIGHT;
				next_pos1 = {pos.x + 1, pos.y};
				next_dir2 = LEFT;
				next_pos2 = {pos.x - 1, pos.y};
				break;

			default:
				throw;
		}

		MinHeatloss& min = min_heatloss[pos.y][pos.x][direction];
		/*
		if (heatloss > min[next_dir1].val && min[next_dir1].steps >= 3)
			return;
		if (heatloss > min[next_dir2].val && min[next_dir2].steps >= 3)
			return;
		*/
		if (heatloss COMPARISON_OP min.val && num_steps_in_direction >= min.steps)
			return;

		if (!ultra_crucible || num_steps_in_direction >= 4) {
			min.val = heatloss;
			min.steps = num_steps_in_direction;
		}

		path.push_back(direction);
		heatloss_along_path.push_back(heatloss);

		if (num_steps_in_direction < (ultra_crucible ? 10 : 3)) {
			Position next_pos = {
				pos.x + (direction == RIGHT ? +1 : (direction == LEFT ? -1 : 0)),
				pos.y + (direction == DOWN ? +1 : (direction == UP ? -1 : 0))
			};
			go_step(next_pos, direction, num_steps_in_direction + 1, heatloss, ultra_crucible);
		}
		if (!ultra_crucible || num_steps_in_direction >= 4) {
			go_step(next_pos1, next_dir1, 1, heatloss, ultra_crucible);
			go_step(next_pos2, next_dir2, 1, heatloss, ultra_crucible);
		}

		path.pop_back();
		heatloss_along_path.pop_back();
	}
};

int main() {
	Heatfield heatfield1;
	Heatfield heatfield2(true);
	return 0;
}
