#include <stdio.h>
#include <iostream>
#include <cstring>
#include <map>
#include <vector>
#include <string>
#include <bits/stdc++.h>
#include <numeric>

using namespace std;


ostream& operator<<(ostream& os, vector<int>& intvector) {
	os << '{';
	for (int i = 0; i < intvector.size(); i++) {
		if (i > 0) {
			os << ", ";
		}
		os << intvector.at(i);
	}
	os << '}';
	return os;
}


int main(int argc, char** argv) {
	char step[1000];
	char dummy[1000];

	map<string, pair<string, string>> desertmap;

	FILE* fp = fopen("input.txt", "r");

	fgets(step, 1000, fp);
	step[strlen(step) - 1] = '\0';
	fgets(dummy, 1000, fp);

	char key[1000], left[1000], right[1000];

	while (!feof(fp)) {
		fscanf(fp, "%s = (%[A-Z], %[A-Z])", key, left, right);
		pair<string, string> dest(left, right);
		desertmap[key] = dest;
	}

	fclose(fp);

	long count = 0;
	string pos = "AAA";
	while (pos != "ZZZ") {
		pair<string, string> dest = desertmap[pos];
		char next_step = step[count % strlen(step)];
		if (next_step == 'L') {
			pos = dest.first;
		} else if (next_step == 'R') {
			pos = dest.second;
		} else {
			throw;
		}
		count++;
	}

	cout << count << endl;

	///////////////////////// Part 2 ////////////////////////////

	bool steps[1000];
	const int num_steps = strlen(step);
	for (int i = 0; i < num_steps; i++) {
		if (step[i] == 'L') {
			steps[i] = true;
		} else if (step[i] == 'R') {
			steps[i] = false;
		} else {
			throw;
		}
	}

	vector<string> pos_list;
	for (pair<string, pair<string, string>> entry : desertmap) {
		pos_list.push_back(entry.first);
	}

	vector<int> positions;
	int iLeft[1000];
	int iRight[1000];
	int end_pos[1000];
	for (pair<string, pair<string, string>> entry : desertmap) {
		vector<string>::iterator it;
		int pos_index = find(pos_list.begin(), pos_list.end(), entry.first) - pos_list.begin();
		int left_index = find(pos_list.begin(), pos_list.end(), entry.second.first) - pos_list.begin();
		int right_index = find(pos_list.begin(), pos_list.end(), entry.second.second) - pos_list.begin();

		iLeft[pos_index] = left_index;
		iRight[pos_index] = right_index;
		end_pos[pos_index] = entry.first.back() == 'Z';

		if (entry.first.back() == 'A') {
			positions.push_back(pos_index);
		}
	}

	vector<vector<int>> all_endpositions;
	vector<int> loop_lengths;
	vector<int> offsets;
	for (int pos : positions) {
		const int startpos = pos;
		count = 0;
		map<pair<int, int>, int> visited;
		vector<int> endpositions;
		while (true) {
			int step = count % num_steps;
			if (end_pos[pos]) {
				endpositions.push_back(count);
			}
			pair<int, int> steppos = make_pair(step, pos);
			map<pair<int, int>, int>::iterator it;
			it = visited.find(steppos);
			if (it != visited.end()) {
				int offset = it->second;
				int looplength = count - it->second;

				offsets.push_back(offset);
				loop_lengths.push_back(looplength);
				all_endpositions.push_back(endpositions);

				cout << "For startpos " << startpos << ": Offset ";
				cout << offset << ", loop: " << looplength;
				cout << ", endpositions: " << endpositions << endl;
				break;
			}
			visited[steppos] = count;
			if (steps[step]) {
				pos = iLeft[pos];
			} else {
				pos = iRight[pos];
			}
			count++;
		}
	}

	bool special_case = true;
	for (int i = 0; i < positions.size(); i++) {
		if (all_endpositions[i].size() != 1 || loop_lengths[i] != all_endpositions[i][0]) {
			special_case = false;
		}
	}
	if (special_case) {
		cout << "We have a special case, we have exactly one end position for ";
		cout << "all the loops after every start position, and all loop ";
		cout << "lengths are equal to first endposition encounter. That ";
		cout << "makes calculation very easy!" << endl;

		long long moves = 1;
		for (int looplength : loop_lengths) {
			moves = lcm(moves, looplength);
		}
		cout << moves << endl;
		exit(0);
	}


	//////////////////// Hardcore brute force method //////////////

	while (true) {
		bool end_reached = true;
		for (int& pos : positions) {
			if (steps[count % num_steps]) {
				pos = iLeft[pos];
			} else {
				pos = iRight[pos];
			}
			end_reached = end_reached && end_pos[pos];
		}
		count++;
		if (end_reached) {
			break;
		}
		if ((count % 1000000000) == 0) {
			cout << count << endl;
		}
	}

	cout << count << endl;
}
