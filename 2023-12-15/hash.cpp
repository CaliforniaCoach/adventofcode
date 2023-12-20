#include <iostream>
#include <fstream>
#include <vector>

using namespace std;


int main() {
	ifstream file("input.txt");
	long sum = 0;
	int hash = 0;
	int count = 0;
	while (file.good()) {
		int c = file.get();
		if (c != EOF && c != ',' && c != '\n') {
			hash = (hash + c) * 17;
			hash %= 256;
		} else {
			sum += hash;
			hash = 0;
			count ++;
		}
	}
	file.close();

	cout << count << " sequences: " << sum << endl << endl;


	file.open("input.txt");
	vector<vector<pair<string, int>>> boxes(256, vector<pair<string, int>>());
	hash = 0;
	string label = "";
	while (file.good()) {
		int c = file.get();
		if (c != EOF && c != ',' && c != '\n' && c != '=' && c != '-') {
			hash = (hash + c) * 17;
			hash %= 256;
			label.push_back(c);
		} else if (c == '=') {
			c = file.get();
			int val = c - '0';
			bool found = false;
			for (int i = 0; i < boxes[hash].size(); i++) {
				if (boxes[hash][i].first == label) {
					boxes[hash][i].second = val;
					found = true;
					break;
				}
			}
			if (!found) {
				boxes[hash].push_back(make_pair(label, val));
			}
			c = file.get();
			if (c != ',' && c != '\n' && c != EOF) {
				throw;
			}
			hash = 0;
			label = "";
		} else if (c == '-') {
			for (int i = 0; i < boxes[hash].size(); i++) {
				if (boxes[hash][i].first == label) {
					boxes[hash].erase(boxes[hash].begin() + i);
					break;
				}
			}
			c = file.get();
			if (c != ',' && c != '\n' && c != EOF) {
				throw;
			}
			hash = 0;
			label = "";
		} else if (c == ',') {
			throw;
		}
	}
	file.close();

	int power = 0;
	for (int i = 0; i < boxes.size(); i++) {
		if (boxes[i].size() != 0) {
			cout << "Box " << i << ":" << endl;
			for (int k = 0; k < boxes[i].size(); k++) {
				const pair<string, int>& lens = boxes[i][k];
				cout << "[" << lens.first << " " << lens.second << "] ";
				power += (i + 1) * (k + 1) * lens.second;
			}
			cout << endl << endl;
		}
	}
	cout << "focusing power: " << power << endl;
}
