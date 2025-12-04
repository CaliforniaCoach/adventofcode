#include <iostream>
#include <fstream>
#include <vector>

using namespace std;


int remove_rolls(vector<string>& rows_to_remove_from) {
	vector<string> rows(rows_to_remove_from);
	int count = 0;
	for (int row = 0; row < rows.size(); row++) {
		for (int col = 0; col < rows[row].length(); col++) {
			if (rows[row][col] == '@') {
				int adjacent_rolls = 0;
				for (int dx = -1; dx <= 1; dx++) {
					for (int dy = -1; dy <= 1; dy++) {
						if (dx || dy) {
							int x = col + dx;
							int y = row + dy;
							if (y >= 0 && y < rows.size() && x >= 0 && x < rows[y].length()) {
								if (rows[y][x] == '@') {
									adjacent_rolls++;
								}
							}
						}
					}
				}
				if (adjacent_rolls < 4) {
					count++;
					rows_to_remove_from[row][col] = '.';
				}
			}
		}
	}
	return count;
}

int main() {
	vector<string> rows;
	string row;
	ifstream ifs("input.txt");
	while (ifs >> row) {
		rows.push_back(row);
	}
	ifs.close();

	int num_removed = remove_rolls(rows);
	cout << num_removed << endl;

	int total_num_removed = num_removed;
	while (num_removed) {
		num_removed = remove_rolls(rows);
		total_num_removed += num_removed;
	}
	cout << total_num_removed << endl;
}
