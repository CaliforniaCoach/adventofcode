#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;


#define MAX(X, Y) (((X) < (Y)) ? (Y) : (X))


int find_reflection(const vector<string>& field, int min_row, int max_row) {
	int sum = 0;
	for (int row = MAX(1, min_row); row < field.size() && row <= max_row; row++) {
		bool reflection = true;
		for (int testrow = 0; testrow < row; testrow++) {
			int reflectrow = row * 2 - testrow - 1;
			if (reflectrow < field.size()) {
				if (field[testrow] != field[reflectrow]) {
					reflection = false;
					break;
				}
			}
		}
		if (reflection) {
			sum += row;
		}
	}
	return sum;
}

vector<string> transpose(const vector<string>& field) {
	vector<string> newfield(field[0].size(), "");
	for (const string& line : field) {
		for (int i = 0; i < line.size(); i++) {
			newfield[i].push_back(line[i]);
		}
	}
	return newfield;
}

int find_reflections(const vector<string>& field, int relevant_x = -1, int relevant_y = -1) {
	int min_x = relevant_x >= 0 ? relevant_x : 0;
	int min_y = relevant_y >= 0 ? relevant_y : 0;
	int max_x = relevant_x >= 0 ? (field[0].size() + relevant_x) / 2 : 99999;
	int max_y = relevant_y >= 0 ? (field.size() + relevant_y) / 2 : 99999;

	int sum = 100 * find_reflection(field, min_y, max_y);
	vector<string> transposed = transpose(field);
	return sum + find_reflection(transposed, min_x, max_x);

}

int find_smudged_reflections(vector<string>& field) {
	for (int y = 0; y < field.size(); y++) {
		for (int x = 0; x < field[y].size(); x++) {
			field[y][x] = field[y][x] == '#' ? '.' : '#';
			int score = find_reflections(field, x, y);
			if (score > 0) {
				return score;
			}
			field[y][x] = field[y][x] == '#' ? '.' : '#';
		}
	}
	throw;
}


int main() {
	vector<string> field;
	string line;
	int score = 0;
	ifstream file("input.txt");
	while (getline(file, line)) {
		if (line.size() > 0) {
			field.push_back(line);
		} else {
			score += find_reflections(field);
			field.clear();
		}
	}
	file.close();
	score += find_reflections(field);

	cout << score << endl;

	score = 0;
	field.clear();
	file.open("input.txt");
	while (getline(file, line)) {
		if (line.size() > 0) {
			field.push_back(line);
		} else {
			score += find_smudged_reflections(field);
			field.clear();
		}
	}
	file.close();
	score += find_smudged_reflections(field);

	cout << score << endl;
}
