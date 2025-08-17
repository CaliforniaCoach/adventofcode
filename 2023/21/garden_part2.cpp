#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>

using namespace std;


#define FILENAME "input.txt"
const int STEPS = 26501365;
// const long STEPS = 12345678;


ostream& operator<<(ostream& os, const vector<vector<int>>& vec) {
	for (const vector<int> row : vec) {
		for (int x : row) {
			cout << setw(4) << (x < 0 ? '#' : x);
		}
		cout << endl;
	}
	return os;
}

void step(vector<vector<int>>& field, int x, int y, int stepcount, int max_steps = -1) {
	if (y < 0 || y >= field.size() || x < 0 || y >= field[0].size())
		return;
	if (field[y][x] <= -2)
		return;

	if (field[y][x] >= 0 && field[y][x] <= stepcount)
		return;

	field[y][x] = stepcount;
	if (max_steps < 0 || stepcount < max_steps) {
		step(field, x + 1, y, stepcount + 1, max_steps);
		step(field, x - 1, y, stepcount + 1, max_steps);
		step(field, x, y + 1, stepcount + 1, max_steps);
		step(field, x, y - 1, stepcount + 1, max_steps);
	}
}

int count_reached_fields(const vector<vector<int>>& field, bool even) {
	int count = 0;
	for (const vector<int>& row : field)
		for (int x : row)
			if (x >= 0 && x % 2 == (even ? 0 : 1))
				count++;
	return count;
}

int get_reachable_fields(const vector<vector<int>>& field, int start_x, int start_y, int max_steps) {
	if (max_steps < 0)
		return 0;
	vector<vector<int>> field_copy(field);
	step(field_copy, start_x, start_y, 0, max_steps);
	return count_reached_fields(field_copy, (max_steps & 0b1) == 0);
}

int get_total_reachable_fields(const vector<vector<int>>& field, int start_x, int start_y, bool even) {
	vector<vector<int>> field_copy(field);
	step(field_copy, start_x, start_y, 0, -1);
	return count_reached_fields(field_copy, even);
}


int main() {
	vector<vector<int>> field;
	ifstream file(FILENAME);
	string line;
	int startx = -1, starty = -1;
	while (getline(file, line)) {
		vector <int> row;
		for (char chr : line) {
			if (chr == 'S') {
				startx = row.size();
				starty = field.size();
			}
			row.push_back(chr == '#' ? -2 : -1);
		}
		field.push_back(row);
	}
	file.close();

	long long map_size = field.size();
	if (map_size != field[0].size())
		throw;

	bool stepnumber_even = (STEPS & 0b1) == 0;

	const vector<vector<int>> field_backup(field);
	step(field, startx, starty, 0);

	int steps_to_northeast = field[0][map_size - 1];
	int steps_to_northwest = field[0][0];
	int steps_to_southeast = field[map_size - 1][map_size - 1];
	int steps_to_southwest = field[map_size - 1][0];

	int reached_fields_even = count_reached_fields(field, stepnumber_even);

	// Let's assume that corner plots are all reachable in same number of steps,
	// makes calculations from now on a lot easier
	if (steps_to_northeast != steps_to_northwest || steps_to_northwest != steps_to_southeast
		|| steps_to_southeast != steps_to_southwest)
		throw;

	// Repeat calculation, but now for odd maps, i.e. start pos is odd
	// number of steps away from original start pos
	int startx_odd = startx + 1;
	while (field_backup[starty][startx_odd] <= -2) {
		startx_odd -= 2;
		if (startx_odd < 0) {
			throw;
		}
	}
	int reached_fields_odd = get_total_reachable_fields(field_backup, startx_odd, starty, stepnumber_even);

	// Calc number of maps of which all plots can be reached in NW, NE, SW and SE directions
	long fully_reachable_maps_northeast_max_row = (STEPS - steps_to_northeast - 1) / map_size - 1;


	long long fully_reachable_maps_odd, fully_reachable_maps_even;
	if ((fully_reachable_maps_northeast_max_row & 0b1) == 0) {
		long long n = fully_reachable_maps_northeast_max_row / 2;
		// 1 + 1 + 2 + 2 + ... + n + n  =  sum(2i), i = 1 to n  =  n * (n + 1)
		fully_reachable_maps_odd = 4 * n * (n + 1);
		// 1 + 1 + 2 + 2 + ... + n      =  n * (n + 1)  =  n^2
		fully_reachable_maps_even = 4 * n * n;
	} else {
		long long n = (fully_reachable_maps_northeast_max_row + 1) / 2;
		// Like above, but the other way round
		fully_reachable_maps_even = 4 * n * n;
		fully_reachable_maps_odd = 4 * n * (n - 1);
	}

	int remaining_steps_small = (STEPS - steps_to_northeast - 1) % map_size;
	int remaining_steps_big = remaining_steps_small + map_size;
	int remaining_num_maps_big = fully_reachable_maps_northeast_max_row + 1;
	int remaining_num_maps_small = fully_reachable_maps_northeast_max_row + 2;

	int remaining_reachable_small_ne = get_reachable_fields(field_backup, 0,            map_size - 1, remaining_steps_small - 1);
	int remaining_reachable_small_nw = get_reachable_fields(field_backup, map_size - 1, map_size - 1, remaining_steps_small - 1);
	int remaining_reachable_small_se = get_reachable_fields(field_backup, 0,            0,            remaining_steps_small - 1);
	int remaining_reachable_small_sw = get_reachable_fields(field_backup, map_size - 1, 0,            remaining_steps_small - 1);

	int remaining_reachable_big_ne = get_reachable_fields(field_backup, 0,            map_size - 1, remaining_steps_big - 1);
	int remaining_reachable_big_nw = get_reachable_fields(field_backup, map_size - 1, map_size - 1, remaining_steps_big - 1);
	int remaining_reachable_big_se = get_reachable_fields(field_backup, 0,            0,            remaining_steps_big - 1);
	int remaining_reachable_big_sw = get_reachable_fields(field_backup, map_size - 1, 0,            remaining_steps_big - 1);
	
	// Now do calculations for N, E, S, W directions

	// Ensure there are "highways" from start position to N, E, S, W edges of map
	for (int x = 0; x < map_size; x++)
		if (field_backup[starty][x] <= -2)
			throw;
	for (int y = 0; y < map_size; y++)
		if (field_backup[y][startx] <= -2)
			throw;
	
	// Ensure number of steps to edges are all the same in all directions
	int steps_to_edge_n = field[starty][0];
	int steps_to_edge_s = field[starty][map_size - 1];
	int steps_to_edge_w = field[0][startx];
	int steps_to_edge_e = field[map_size - 1][startx];
	if (steps_to_edge_n != steps_to_edge_s || steps_to_edge_s != steps_to_edge_w ||
	    steps_to_edge_w != steps_to_edge_e)
		throw;

	int fully_reachable_maps_n_e_s_w = (STEPS - steps_to_edge_n - steps_to_edge_e) / map_size;

	int fully_reachable_maps_n_e_s_w_even = fully_reachable_maps_n_e_s_w / 2;
	int fully_reachable_maps_n_e_s_w_odd = (fully_reachable_maps_n_e_s_w + 1) / 2;

	int remaining_small_n_e_s_w = (STEPS - steps_to_edge_n) % map_size;
	int remaining_big_n_e_s_w = remaining_small_n_e_s_w < steps_to_edge_e ? remaining_small_n_e_s_w + map_size : 0;

	int remaining_reachable_small_n = get_reachable_fields(field_backup, startx, map_size - 1, remaining_small_n_e_s_w - 1);
	int remaining_reachable_small_s = get_reachable_fields(field_backup, startx, 0,            remaining_small_n_e_s_w - 1);
	int remaining_reachable_small_w = get_reachable_fields(field_backup, map_size - 1, starty, remaining_small_n_e_s_w - 1);
	int remaining_reachable_small_e = get_reachable_fields(field_backup, 0,            starty, remaining_small_n_e_s_w - 1);

	int remaining_reachable_big_n = get_reachable_fields(field_backup, startx, map_size - 1, remaining_big_n_e_s_w - 1);
	int remaining_reachable_big_s = get_reachable_fields(field_backup, startx, 0,            remaining_big_n_e_s_w - 1);
	int remaining_reachable_big_w = get_reachable_fields(field_backup, map_size - 1, starty, remaining_big_n_e_s_w - 1);
	int remaining_reachable_big_e = get_reachable_fields(field_backup, 0,            starty, remaining_big_n_e_s_w - 1);


	// Sum up everything

	long total = fully_reachable_maps_even * reached_fields_even + fully_reachable_maps_odd * reached_fields_odd;
	total += 4 * ((long)fully_reachable_maps_n_e_s_w_even * reached_fields_even + (long)fully_reachable_maps_n_e_s_w_odd * reached_fields_odd);
	total += (long)remaining_num_maps_big   * (remaining_reachable_big_ne   + remaining_reachable_big_nw   + remaining_reachable_big_se   + remaining_reachable_big_sw);
	total += (long)remaining_num_maps_small * (remaining_reachable_small_ne + remaining_reachable_small_nw + remaining_reachable_small_se + remaining_reachable_small_sw);
	total += remaining_reachable_small_n + remaining_reachable_small_s + remaining_reachable_small_w + remaining_reachable_small_e;
	total += remaining_reachable_big_n + remaining_reachable_big_s + remaining_reachable_big_w + remaining_reachable_big_e;
	total += reached_fields_even;

	cout << total << endl;
}
