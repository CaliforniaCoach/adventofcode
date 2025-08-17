#include <iostream>
#include <fstream>
#include <vector>

using namespace std;


#define FILENAME "input.txt"
#define STEPS 64


ostream& operator<<(ostream& os, const vector<vector<int>>& vec) {
	for (const vector<int> row : vec) {
		for (int x : row) {
			cout << (x < 0 ? '#' : (x > 0 && x % 2 == 0) ? 'O' : '.');
		}
		cout << endl;
	}
	return os;
}

void step(vector<vector<int>>& field, int x, int y, int stepcount) {
	if (y < 0 || y >= field.size() || x < 0 || y >= field[0].size())
		return;
	if (field[y][x] <= -2)
		return;

	if (field[y][x] >= 0 && field[y][x] <= stepcount)
		return;

	field[y][x] = stepcount;
	if (stepcount < STEPS) {
		step(field, x + 1, y, stepcount + 1);
		step(field, x - 1, y, stepcount + 1);
		step(field, x, y + 1, stepcount + 1);
		step(field, x, y - 1, stepcount + 1);
	}
}

int count_reached_fields(const vector<vector<int>>& field) {
	int count = 0;
	for (const vector<int>& row : field)
		for (int x : row)
			if (x >= 0 && x % 2 == 0)
				count++;
	return count;
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

	step(field, startx, starty, 0);

	// cout << field << endl;
	cout << count_reached_fields(field) << endl;
}
