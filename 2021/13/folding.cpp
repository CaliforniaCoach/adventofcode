#include <iostream>
#include <fstream>
#include <set>
#include <vector>

using namespace std;


#define ABS(x) ((x) > 0 ? (x) : -(x))

set<pair<int, int>> fold(set<pair<int, int>>& dots, int coord, bool vertical) {
	set<pair<int, int>> result_set;
	for (const pair<int, int>& dot : dots) {
		int x = dot.first;
		int y = dot.second;
		if (vertical) {
			x = coord - ABS(coord - x);
		} else {
			y = coord - ABS(coord - y);
		}
		result_set.insert({x, y});
	}
	return result_set;
}

int main() {
	set<pair<int, int>> dots;
	vector<pair<bool, int>> folding_instructions;
	int x, y;
	char chr;
	ifstream ifs("input.txt");
	while (ifs >> x >> chr >> y) {
		dots.insert({x, y});
	}
	ifs.clear();
	string str;
	char separator;
	int coord;
	while (ifs >> str >> str >> chr >> separator >> coord) {
		folding_instructions.push_back({chr == 'x', coord});
	};
	ifs.close();

	coord = folding_instructions[0].second;
	bool vertical = folding_instructions[0].first;
	cout << fold(dots, coord, vertical).size() << endl;

	for (const pair<bool, int> folding_instruction : folding_instructions) {
		coord = folding_instruction.second;
		bool vertical = folding_instruction.first;
		dots = fold(dots, coord, vertical);
	}

	int img[100][100] = {};
	int max_x = 0;
	int max_y = 0;
	for (const pair<int, int> dot : dots) {
		img[dot.second][dot.first] = 1;
		max_x = dot.first > max_x ? dot.first : max_x;
		max_y = dot.second > max_y ? dot.second : max_y;
	}
	for (int y = 0; y <= max_y; y++) {
		for (int x = 0; x <= max_x; x++) {
			cout << (img[y][x] ? "##" : "  ");
		}
		cout << endl;
	}
}
