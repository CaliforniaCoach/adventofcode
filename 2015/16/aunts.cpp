#include <iostream>
#include <fstream>
#include <map>

using namespace std;


template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

int main() {
	const map<string, int> mfcsam = {
		{"children:", 3},
		{"cats:", 7},
		{"samoyeds:", 2},
		{"pomeranians:", 3},
		{"akitas:", 0},
		{"vizslas:", 0},
		{"goldfish:", 5},
		{"trees:", 3},
		{"cars:", 2},
		{"perfumes:", 1},
	};

	const map<string, int> sign = {
		{"children:", 0},
		{"cats:", +1},
		{"samoyeds:", 0},
		{"pomeranians:", -1},
		{"akitas:", 0},
		{"vizslas:", 0},
		{"goldfish:", -1},
		{"trees:", +1},
		{"cars:", 0},
		{"perfumes:", 0},
	};

	ifstream file("input.txt");
	while (!file.eof()) {
		int aunt_no, no1, no2, no3;
		string aunt, thing1, thing2, thing3, dummy;
		file >> aunt >> aunt_no >> dummy;
		file >> thing1 >> no1 >> dummy;
		file >> thing2 >> no2 >> dummy;
		file >> thing3 >> no3 >> ws;

		if (aunt != "Sue") {
			throw runtime_error("Unexpected aunt");
		}

		if (mfcsam.at(thing1) == no1 && mfcsam.at(thing2) == no2 && mfcsam.at(thing3) == no3) {
			cout << "Exact match: Aunt Sue " << aunt_no << endl;
		}

		if (sgn(no1 - mfcsam.at(thing1)) == sign.at(thing1) &&
			sgn(no2 - mfcsam.at(thing2)) == sign.at(thing2) &&
			sgn(no3 - mfcsam.at(thing3)) == sign.at(thing3)
		) {
			cout << "Fuzzy match: Aunt Sue " << aunt_no << endl;
		}
	}
	file.close();
}
