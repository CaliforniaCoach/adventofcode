#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;


/*
full cycles       = total / (duration + resttime)
remaining flytime = min(duration, total % (duration + resttime))
*/


int main() {
	vector<int> speed, duration, rest;
	string line, dummy;

	int total = 2503;
	int maxdist = 0;

	ifstream file("input.txt");
	while (!file.eof()) {
		int sp, re, du;
		file >> dummy >> dummy >> dummy >> sp >> dummy >> dummy >> du >> dummy;
		file >> dummy >> dummy >> dummy >> dummy >> dummy >> re >> dummy;

		speed.push_back(sp);
		duration.push_back(du);
		rest.push_back(re);

		int dist = sp * ((total / (du + re)) * du + min(du, total % (du + re)));
		maxdist = max(maxdist, dist);
	}
	file.close();

	cout << maxdist << endl;


	/////////////// Part 2 //////////////////

	vector<int> points(speed.size(), 0);
	vector<int> pos(speed.size(), 0);

	for (int i = 0; i < total; i++) {
		int leadpos = 0;
		for (int k = 0; k < speed.size(); k++) {
			if (i % (duration[k] + rest[k]) < duration[k]) {
				pos[k] += speed[k];
			}
			leadpos = max(leadpos, pos[k]);
		}
		for (int k = 0; k < speed.size(); k++) {
			if (pos[k] == leadpos) {
				points[k]++;
			}
		}
	}

	int maxpoints = 0;
	for (int k = 0; k < points.size(); k++) {
		maxpoints = max(maxpoints, points[k]);
	}

	cout << maxpoints << endl;
}
