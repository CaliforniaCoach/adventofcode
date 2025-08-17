#include <iostream>
#include <fstream>
#include <vector>
#include <set>

using namespace std;


int main() {
	vector<int> deltas;
	set<int> freqs;
	ifstream fs("input.txt");
	long freq = 0;
	while (true) {
		long delta;
		fs >> delta;
		if (fs.eof()) {
			break;
		} else {
			freq += delta;
			deltas.push_back(delta);
		}
	}
	fs.close();

	cout << "Sum after one run: " << freq << endl;

	freq = 0;
	while (true) {
		for (int delta: deltas) {
			freq += delta;
			if (freqs.find(freq) == freqs.end()) {
				freqs.insert(freq);
			} else {
				goto loop_exit;
			}
		}
	}
	loop_exit:
	cout << "First freqeuncy seen twice: " << freq << endl;
}
