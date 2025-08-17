#include <iostream>
#include <fstream>
#include <vector>
#include <map>

using namespace std;


pair<vector<vector<int>>, vector<vector<int>>> calc_blockers(const vector<string>& field) {
	vector<vector<int>> blockers_x(field[0].size(), vector<int>(1, -1));
	vector<vector<int>> blockers_y(field.size(), vector<int>(1, -1));

	for (int y = 0; y < field.size(); y++)
		for (int x = 0; x < field.size(); x++)
			if (field[y][x] == '#') {
				blockers_x[x].push_back(y);
				blockers_y[y].push_back(x);
			}

	for (vector<int>& blockers : blockers_x)
		blockers.push_back(field.size());

	for (vector<int>& blockers : blockers_y)
		blockers.push_back(field[0].size());

	return make_pair(blockers_x, blockers_y);
}

void tilt_north(vector<string>& field, const vector<vector<int>>& blockers) {
	for (int x = 0; x < field[0].size(); x++) {
		const vector<int>& blockpoints = blockers[x];
		for (int i = 0; i < blockpoints.size() - 1; i++) {
			int range_start = blockpoints[i] + 1;
			int range_end = blockpoints[i + 1] - 1;
			int rocks_count = 0;
			for (int y = range_start; y <= range_end; y++) {
				if (field[y][x] == 'O') {
					rocks_count++;
				}
			}
			int y = range_start;
			while (rocks_count-- > 0) {
				field[y++][x] = 'O';
			}
			while (y <= range_end) {
				field[y++][x] = '.';
			}
		}
	}
}

void tilt_west(vector<string>& field, const vector<vector<int>>& blockers) {
	for (int y = 0; y < field.size(); y++) {
		const vector<int>& blockpoints = blockers[y];
		for (int i = 0; i < blockpoints.size() - 1; i++) {
			int range_start = blockpoints[i] + 1;
			int range_end = blockpoints[i + 1] - 1;
			int rocks_count = 0;
			for (int x = range_start; x <= range_end; x++) {
				if (field[y][x] == 'O') {
					rocks_count++;
				}
			}
			int x = range_start;
			while (rocks_count-- > 0) {
				field[y][x++] = 'O';
			}
			while (x <= range_end) {
				field[y][x++] = '.';
			}
		}
	}
}

void tilt_south(vector<string>& field, const vector<vector<int>>& blockers) {
	for (int x = 0; x < field[0].size(); x++) {
		const vector<int>& blockpoints = blockers[x];
		for (int i = 0; i < blockpoints.size() - 1; i++) {
			int range_start = blockpoints[i] + 1;
			int range_end = blockpoints[i + 1] - 1;
			int rocks_count = 0;
			for (int y = range_start; y <= range_end; y++) {
				if (field[y][x] == 'O') {
					rocks_count++;
				}
			}
			int y = range_end;
			while (rocks_count-- > 0) {
				field[y--][x] = 'O';
			}
			while (y >= range_start) {
				field[y--][x] = '.';
			}
		}
	}
}

void tilt_east(vector<string>& field, const vector<vector<int>>& blockers) {
	for (int y = 0; y < field.size(); y++) {
		const vector<int>& blockpoints = blockers[y];
		for (int i = 0; i < blockpoints.size() - 1; i++) {
			int range_start = blockpoints[i] + 1;
			int range_end = blockpoints[i + 1] - 1;
			int rocks_count = 0;
			for (int x = range_start; x <= range_end; x++) {
				if (field[y][x] == 'O') {
					rocks_count++;
				}
			}
			int x = range_end;
			while (rocks_count-- > 0) {
				field[y][x--] = 'O';
			}
			while (x >= range_start) {
				field[y][x--] = '.';
			}
		}
	}
}

void tilt_west(vector<string>& field) {
	for (int y = 0; y < field.size(); y++) {
		for (int x = 0; x < field[y].size(); x++) {
			if (field[y][x] == 'O') {
				int move_x = x - 1;
				while (move_x >= 0 && field[y][move_x] == '.')
					move_x--;
				field[y][x] = '.';
				field[y][move_x + 1] = 'O';
			}
		}
	}
}

void tilt_south(vector<string>& field) {
	for (int y = field.size() - 1; y >= 0; y--) {
		for (int x = 0; x < field[y].size(); x++) {
			if (field[y][x] == 'O') {
				int move_y = y + 1;
				while (move_y < field.size() && field[move_y][x] == '.')
					move_y++;
				field[y][x] = '.';
				field[move_y - 1][x] = 'O';
			}
		}
	}
}

void tilt_east(vector<string>& field) {
	for (int y = 0; y < field.size(); y++) {
		for (int x = field[y].size(); x >= 0; x--) {
			if (field[y][x] == 'O') {
				int move_x = x + 1;
				while (move_x < field[y].size() && field[y][move_x] == '.')
					move_x++;
				field[y][x] = '.';
				field[y][move_x - 1] = 'O';
			}
		}
	}
}

int calc_load(const vector<string>& field) {
	int load = 0;
	for (int y = 0; y < field.size(); y++) {
		for (int x = 0; x < field[y].size(); x++) {
			if (field[y][x] == 'O') {
				load += field.size() - y;
			}
		}
	}
	return load;
}


int main() {
	string line;
	vector<string> field;
	ifstream file("input.txt");
	while (getline(file, line)) {
		field.push_back(line);
	}
	file.close();

	pair<vector<vector<int>>, vector<vector<int>>> blockers = calc_blockers(field);

	map<vector<string>, int> all_fields;
	tilt_north(field, blockers.first);

	cout << calc_load(field) << endl;

	tilt_west(field, blockers.second);
	tilt_south(field, blockers.first);
	tilt_east(field, blockers.second);
	for (int i = 1; i < 1000000000; i++) {
		tilt_north(field, blockers.first);
		tilt_west(field, blockers.second);
		tilt_south(field, blockers.first);
		tilt_east(field, blockers.second);

		pair<map<vector<string>, int>::iterator, bool> retval;
		retval = all_fields.insert(make_pair(field, i));
		if (!retval.second) {
			cout << retval.first->second << " " << i << endl;

			int cycle = retval.first->second - i;
			int skip_cycles = (1000000000 - i) / cycle;
			i += skip_cycles * cycle;

			all_fields.clear();
		}
	}

	cout << calc_load(field) << endl;
}
