#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <algorithm>

using namespace std;


const string FILENAME = "input.txt";


int backwards(const vector<pair<string, string>>& replacements, map<string, int>& already_met,
			string medicine, int steps, int min_steps, int max_steps) {

	if (steps++ >= max_steps) {
		return min_steps;
	}
	int replace_count = 0;
	for (const pair<string, string>& replacement : replacements) {
		size_t pos = -1;
		while (true) {
			pos++;
			pos = medicine.find(replacement.second, pos);
			if (pos == string::npos) {
				break;
			}
			replace_count++;
			/*
			if (steps <= 64) {
				cout << "Step " << steps << ", processing replacement " << replace_count << endl;
			}
			*/
			string dest_medicine = medicine;
			dest_medicine.replace(pos, replacement.second.size(), replacement.first);
			if (replacement.first == "e") {
				if (dest_medicine == "e") {
					cout << "Found one: " << steps << endl;
					// return min(min_steps, steps);
					exit(0);
				}
				else {
					// If something is replaced with "e", result must be exactly "e", otherwise
					// it does not make sense to continue
					continue;
				}
			}
			map<string, int>::iterator it = already_met.find(dest_medicine);
			if (it == already_met.end()) {
				already_met.insert(make_pair(dest_medicine, steps));
			}
			else {
				if (steps >= it->second) {
					continue;
				}
				else {
					it->second = steps;
				}
			}
			min_steps = backwards(replacements, already_met, dest_medicine, steps, min_steps, max_steps);
		}
	}
	return min_steps;
}


bool contains_itself(string str1, string str2) {
	for (int i = 0; i < str2.size(); i += str1.size()) {
		if (str2.substr(i, str1.size()) != str1) {
			return false;
		}
	}
	return true;
}


bool sortfunc(const pair<string, string>& item1, const pair<string, string>& item2) {
	if (contains_itself(item1.first, item1.second)) {
		return true;
	}
	if (contains_itself(item2.first, item2.second)) {
		return false;
	}
	return item1.second.size() - item1.first.size() > item2.second.size() - item2.first.size();
}


int main() {
	vector<pair<string, string>> replacements;
	set<string> dest_medicines;

	ifstream file(FILENAME);
	string medicine;
	while (!file.eof()) {
		string src, op, dest;
		file >> src >> op >> dest >> ws;
		if (op == "=>") {
			if (dest.find('e') != string::npos) {
				throw runtime_error("Algo only works if 'e' is never on right side");
			}
			replacements.push_back(make_pair(src, dest));
		}
		else {
			medicine = src;
		}
	}
	file.close();

	for (const pair<string, string>& replacement : replacements) {
		size_t pos = 0;
		while (true) {
			pos = medicine.find(replacement.first, pos);
			if (pos == string::npos) {
				break;
			}
			string dest_medicine = medicine;
			dest_medicine.replace(pos, replacement.first.size(), replacement.second);
			dest_medicines.insert(dest_medicine);
			pos++;
		}
	}

	cout << dest_medicines.size() << endl;


	//////////////////// Part 2 ////////////////////

	sort(replacements.begin(), replacements.end(), sortfunc);

	int max_steps = 9999;
	while(true) {
		int very_high_value = 9999999;
		map<string, int> already_met;
		cout << "Trying with max. " << ++max_steps << " steps..." << endl;
		backwards(replacements, already_met, medicine, 0, very_high_value, max_steps);
		cout << "    ... no solution found." << endl;
	}
}
