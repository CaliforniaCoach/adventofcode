#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <map>
#include <set>
#include <cassert>

using namespace std;

class Relpos {
	public:
		array<int, 3> relpos;

		Relpos rotate(int rotation) const {
			Relpos result = *this;
			switch (rotation % 3) {
				case 0: result = {result[0], result[1], result[2]}; break;
				case 1: result = {result[1], result[2], result[0]}; break;
				case 2: result = {result[2], result[0], result[1]}; break;
			}
			switch ((rotation / 3) % 4) {
				case 0: result = {result[0], result[1], result[2]}; break;
				case 1: result = {result[0], result[2], -result[1]}; break;
				case 2: result = {result[0], -result[1], -result[2]}; break;
				case 3: result = {result[0], -result[2], result[1]}; break;
			}
			switch (rotation / 12) {
				case 0: result = {result[0], result[1], result[2]}; break;
				case 1: result = {-result[0], -result[2], -result[1]}; break;
				default: assert(false);
			}
			return result;
		}

		int& operator[](int idx) {
			return relpos[idx];
		}

		const int& operator[](int idx) const {
			return relpos[idx];
		}

		array<int, 3> operator-(const Relpos& relpos2) const {
			return {(*this)[0] - relpos2[0], (*this)[1] - relpos2[1], (*this)[2] - relpos2[2]};
		}
};


class Scanner {
	public:
		vector<Relpos> relpos;
		array<int, 3> scanner_pos;
		int orientation = -1;

		bool match(const Scanner& scanner2) {
			for (int rotation = 0; rotation < 24; rotation++) {
				if (match(scanner2, rotation)) {
					return true;
				}
			}
			return false;
		}

		bool match(const Scanner& scanner2, int rotation) {
			int max_matches = 0;
			array<int, 3> max_diff;
			map<array<int, 3>, int> diff_histogram;
			for (const Relpos& rp : relpos) {
				Relpos rotated_rp = rp.rotate(rotation);
				for (const Relpos& rp2 : scanner2.relpos) {
					Relpos rotated_rp2 = rp2.rotate(scanner2.orientation);
					array<int, 3> diff = rotated_rp2 - rotated_rp;
					diff_histogram.insert({diff, 0});
					if (++diff_histogram.at(diff) > max_matches) {
						max_matches = diff_histogram.at(diff);
						max_diff = diff;
					}
				}
			}
			if (max_matches >= 12) {
				scanner_pos = {
					scanner2.scanner_pos[0] + max_diff[0],
					scanner2.scanner_pos[1] + max_diff[1],
					scanner2.scanner_pos[2] + max_diff[2]
				};
				orientation = rotation;
				return true;
			} else {
				return false;
			}
		}
};

int main() {
	vector<Scanner> scanners;
	ifstream ifs("input.txt");
	int num_scanners = 0;
	string title_line;
	while (getline(ifs, title_line)) {
		if (title_line.empty()) {
			continue;
		}
		scanners.emplace_back();
		assert(title_line == "--- scanner " + to_string(num_scanners) + " ---");
		int x, y, z;
		char sep;
		while (ifs >> x >> sep >> y >> sep >> z) {
			scanners[num_scanners].relpos.push_back({x, y, z});
		}
		ifs.clear();
		ifs.seekg(-1, ios_base::cur);
		num_scanners++;
	}
	ifs.close();

	vector<Scanner> scanners_located;
	vector<Scanner> scanners_not_located(scanners);
	scanners_located.push_back(scanners_not_located[0]);
	scanners_located[0].scanner_pos = {0, 0, 0};
	scanners_located[0].orientation = 0;
	scanners_not_located.erase(scanners_not_located.begin());

	while (!scanners_not_located.empty()) {
		for (int i = 0; i < scanners_located.size(); i++) {
			for (int k = 0; k < scanners_not_located.size(); k++) {
				const Scanner& scanner1 = scanners_located[i];
				Scanner& scanner2 = scanners_not_located[k];
				if (scanner2.match(scanner1)) {
					scanners_located.push_back(scanner2);
					scanners_not_located.erase(scanners_not_located.begin() + k--);
				}
			}
		}
	}

	set<array<int, 3>> beacons;
	for (const Scanner& scanner : scanners_located) {
		for (const Relpos& beacon_relpos : scanner.relpos) {
			Relpos rotated_beacon_relpos = beacon_relpos.rotate(scanner.orientation);
			array<int, 3> beacon_pos = {
				scanner.scanner_pos[0] + rotated_beacon_relpos[0],
				scanner.scanner_pos[1] + rotated_beacon_relpos[1],
				scanner.scanner_pos[2] + rotated_beacon_relpos[2]
			};
			beacons.insert(beacon_pos);
		}
	}
	int max_dist = 0;
	for (int i = 0; i < scanners_located.size(); i++) {
		const array<int, 3>& p1 = scanners_located[i].scanner_pos;
		for (int k = i + 1; k < scanners_located.size(); k++) {
			const array<int, 3>& p2 = scanners_located[k].scanner_pos;
			int dist =
			max_dist = max(max_dist, abs(p1[0] - p2[0]) + abs(p1[1] - p2[1]) + abs(p1[2] - p2[2]));
		}
	}
	cout << beacons.size() << endl << max_dist << endl;
}
