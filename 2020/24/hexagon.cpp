#include <iostream>
#include <fstream>
#include <cassert>

using namespace std;


long count(bool grid[500][500]) {
	long count = 0;
	for (int x = 0; x < 500; x++) {
		for (int y = 0; y < 500; y++) {
			if (grid[x][y]) {
				count++;
			}
		}
	}
	return count;
}

int main() {
	// We use two grid coordinates with "odd-r" horizontal layout
	bool grid[500][500]{};
	ifstream fs("input.txt");
	char chr;
	int x = 250;
	int y = 250;
	int flipped = 0;
	int flipped_back = 0;
	while (fs.get(chr)) {
		switch (chr) {
			case '\n':
			assert(x >= 0 && x < 500 && y >= 0 && y < 500);
				grid[x][y] = !grid[x][y];
				flipped += grid[x][y] ? 1 : 0;
				flipped_back += !grid[x][y] ? 1 : 0;
				x = 250;
				y = 250;
				break;
			case 'w':
			case 'e':
				x += chr == 'w' ? -1 : 1;
				break;
			case 'n':
			case 's':
				y += chr == 'n' ? -1 : 1;
				fs.get(chr);
				assert(chr == 'e' || chr == 'w');
				x += chr == 'w' ? -1 : 0;
				if (y % 2 == 0) {
					x++;
				}
				break;
			default:
				assert(false);
		}
	}
	fs.close();
	cout << "Flipped:       " << flipped << endl;
	cout << "Flipped back:  " << flipped_back << endl;
	cout << "Black side up: " << count(grid) << endl;

	for (int day = 1; day <= 100; day++) {
		int num_black_neighbors[500][500] = {0};
		for (int x = 0; x < 500; x++) {
			for (int y = 0; y < 500; y++) {
				if (grid[x][y]) {
					assert(x > 0 && x < 500 - 1 && y > 0 && y < 500 - 1);
					num_black_neighbors[x - 1][y]++;
					num_black_neighbors[x + 1][y]++;
					num_black_neighbors[x][y - 1]++;
					num_black_neighbors[x][y + 1]++;
					num_black_neighbors[x + (y % 2 == 0 ? -1 : 1)][y - 1]++;
					num_black_neighbors[x + (y % 2 == 0 ? -1 : 1)][y + 1]++;
				}
			}
		}
		for (int x = 0; x < 500; x++) {
			for (int y = 0; y < 500; y++) {
				if (grid[x][y] && num_black_neighbors[x][y] == 0 || num_black_neighbors[x][y] > 2) {
					grid[x][y] = false;
				} else if (!grid[x][y] && num_black_neighbors[x][y] == 2) {
					grid[x][y] = true;
				}
			}
		}
	}
	cout << endl << "Black side up after 100 days: " << count(grid) << endl;
}
