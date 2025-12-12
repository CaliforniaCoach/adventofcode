#include <iostream>
#include <fstream>
#include <vector>

using namespace std;


int field[100000][100000] = {0};


int main() {
	vector<pair<long, long>> coords;
	ifstream ifs("input.txt");
	int x, y;
	char chr;
	while (ifs >> x >> chr >> y) {
		coords.push_back({x, y});
	}
	ifs.close();

	long max_area1 = 0;
	long max_area2 = 0;
	for (int i = 0; i < coords.size(); i++) {
		for (int k = i + 1; k < coords.size(); k++) {
			const long& x1 = coords[i].first;
			const long& x2 = coords[k].first;
			const long& y1 = coords[i].second;
			const long& y2 = coords[k].second;
			long area = abs(x1 - x2 + 1) * abs(y1 - y2 + 1);
			max_area1 = max(max_area1, area);
			/*
			bool path1inside = false;
			bool path2inside = false;
			for (int m = i + 1; m <= k - 1; m++) {
				const long& x = coords[m].first;
				const long& y = coords[m].second;
				if (x < min(x1, x2) || x > max(x1, x2) || y < min(y1, y2) || y > max(y1, y2)) {
					path1inside = true;
					break;
				}
			}
			if (path1inside) {
				for (int m = k + 1; m <= coords.size() + i - 1; m++) {
					const long& x = coords[m % coords.size()].first;
					const long& y = coords[m % coords.size()].second;
					if (x < min(x1, x2) || x > max(x1, x2) || y < min(y1, y2) || y > max(y1, y2)) {
						path2inside = true;
						break;
					}
				}
			}
			*/
			long x_prev = coords[coords.size() - 1].first;
			long y_prev = coords[coords.size() - 1].second;
			for (pair<long, long> coord : coords) {
				const long& x = coord.first;
				const long& y = coord.second;
				if (
					(x > min(x1, x2) && x < max(x1, x2) && y > min(y1, y2) && y < max(y1, y2)) ||
					(x > min(x1, x2) && x < max(x1, x2) &&
						((min(y, y_prev) <= min(y1, y2))) && (max(y, y_prev) >= max(y1, y2))) ||
					(y > min(y1, y2) && y < max(y1, y2) &&
						((min(x, x_prev) <= min(x1, x2))) && (max(x, x_prev) >= max(x1, x2)))
				) {
					area = 0;
					break;
				}
				x_prev = x;
				y_prev = y;
			}
			max_area2 = max(max_area2, area);
		}
	}
	cout << max_area1 << endl << max_area2 << endl;
}

// 1652307770 too low
// 2509028080 too high
