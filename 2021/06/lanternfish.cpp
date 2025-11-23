#include <iostream>
#include <fstream>
#include <array>
#include <cassert>

using namespace std;


__uint128_t run(array<__uint128_t, 9> fish, int num_cycles) {
	for (int i = 0; i < num_cycles; i++) {
		__uint128_t new_fish = fish[0];
		for (int i = 0; i < 8; i++) {
			fish[i] = fish[i + 1];
		}
		fish[8] = new_fish;
		fish[6] += new_fish;
	}
	__uint128_t count = 0;
	for (__uint128_t n : fish) {
		count += n;
	}
	return count;
}

int main() {
	array<__uint128_t, 9> fish = {0};
	ifstream ifs("input.txt");
	int val;
	while (ifs >> val) {
		fish[val]++;
		char sep;
		ifs >> sep;
	}
	ifs.close();

	__uint128_t count = run(fish, 80);
	assert(count < __UINT64_MAX__);
	cout << (__uint64_t) count << endl;

	count = run(fish, 256);
	assert(count < __UINT64_MAX__);
	cout << (__uint64_t) count << endl;
}
