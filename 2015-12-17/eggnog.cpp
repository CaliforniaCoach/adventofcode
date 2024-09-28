#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;


template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}


int calc_total(const vector<int>& cap, unsigned long used) {
	int retval = 0;
	for (int i = 0; i < cap.size(); i++) {
		if (used & 1) {
			retval += cap[i];
		}
		used >>= 1;
	}
	return retval;
}


int calc_num_bits(unsigned long num) {
	int retval = 0;
	while (num != 0UL) {
		if (num & 1) {
			retval++;
		}
		num >>= 1;
	}
	return retval;
}


int main() {
	const int TARGET = 150;

	vector<int> cap;

	ifstream file("input.txt");
	while (!file.eof()) {
		int capacity;
		file >> capacity >> ws;
		cap.push_back(capacity);
	}
	file.close();

	int combinations = 0;
	int min_bits = 9999999;
	int num_combinations_min_bits = 0;

	for (unsigned long used = 0UL; used < (1 << cap.size()); used++) {
		if (calc_total(cap, used) == TARGET) {
			combinations++;
			int num_bits = calc_num_bits(used);
			if (num_bits < min_bits) {
				num_combinations_min_bits = 1;
				min_bits = num_bits;
			}
			else if (num_bits == min_bits) {
				num_combinations_min_bits++;
			}
		}
	}

	cout << combinations << endl;
	cout << num_combinations_min_bits << endl;
}
