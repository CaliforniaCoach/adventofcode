#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <climits>

using namespace std;


int main() {
	vector<pair<long, long>> ranges;
	vector<long> ids;
	ifstream ifs("input.txt");
	string line;
	while (getline(ifs, line)) {
		size_t separator = line.find('-');
		if (separator != string::npos) {
			long from = stol(line.substr(0, separator));
			long to = stol(line.substr(separator + 1));
			ranges.push_back(pair<long, long>(from, to));
		} else if (!line.empty()) {
			long id = stol(line);
			assert(id != 0L);
			ids.push_back(id);
		}
	}
	ifs.close();

	for (int i = 0; i < ranges.size(); i++) {
		for (int k = i + 1; k < ranges.size(); k++) {
			long overlap_from = max(ranges[i].first, ranges[k].first);
			long overlap_to = min(ranges[i].second, ranges[k].second);
			if (overlap_to >= overlap_from) {
				ranges[i].first = min(ranges[i].first, ranges[k].first);
				ranges[i].second = max(ranges[i].second, ranges[k].second);
				ranges.erase(ranges.begin() + k);
				// Start again from k = i + 1
				k = i;
			}
		}
	}

	int num_fresh = 0;
	for (long id : ids) {
		for (pair<long, long>& range : ranges) {
			if (id >= range.first && id <= range.second) {
				num_fresh++;
				break;
			}
		}
	}
	cout << num_fresh << endl;

	long long total_ids = 0;
	for (int i = 0; i < ranges.size(); i++) {
		long range_size = ranges[i].second - ranges[i].first + 1;
		assert(LLONG_MAX - range_size > total_ids);
		total_ids += range_size;
	}
	cout << total_ids << endl;
}
