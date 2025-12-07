#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cassert>
#include <algorithm>

using namespace std;


int main() {
	long sum = 0;
	vector<string> lines;
	int max_len = 0;
	vector<vector<int>> numbers;
	ifstream ifs("input.txt");
	string line;
	while (getline(ifs, line)) {
		lines.push_back(line);
		max_len = max(max_len, (int) line.length());
	}
	ifs.close();

	for (string& line : lines) {
		vector<int> line_numbers;
		stringstream sstr(line);
		int num;
		while (sstr >> num) {
			line_numbers.push_back(num);
		}
		if (line_numbers.size() > 0) {
			numbers.push_back(line_numbers);
		} else {
			sstr.clear();
			sstr.seekg(ios_base::beg);
			int col = 0;
			string op;
			while (sstr >> op) {
				long colresult = op == "+" ? 0 : 1;
				for (vector<int> row : numbers) {
					colresult = op == "+" ? colresult + row[col] : colresult * row[col];
				}
				sum += colresult;
				col++;
			}
			assert(col == numbers[0].size());
		}
	}
	cout << sum << endl;

	sum = 0;
	vector<vector<long>> numbers2;
	vector<char> op;
	int num_col = 0;
	for (int col = 0; col < max_len; col++) {
		long num = 0;
		for (int row = 0; row < lines.size() - 1; row++) {
			char digit = lines[row][col];
			assert(digit >= '0' && digit <= '9' || digit == ' ');
			if (digit >= '0' && digit <= '9') {
				num = num * 10 + digit - '0';
			}
		}
		if (num > 0) {
			if (numbers2.size() <= num_col) {
				numbers2.emplace_back();
				op.emplace_back();
			}
			numbers2[num_col].push_back(num);
			if (lines[lines.size() - 1][col] != ' ') {
				op[num_col] = lines[lines.size() - 1][col];
			}
		} else {
			num_col = numbers2.size();
		}
	}
	for (int num_col = 0; num_col < numbers2.size(); num_col++) {
		assert(op[num_col] == '+' || op[num_col] == '*');
		long colresult = op[num_col] == '+' ? 0 : 1;
		for (long num : numbers2[num_col]) {
			colresult = op[num_col] == '+' ? colresult + num : colresult * num;
		}
		sum += colresult;
	}
	cout << sum << endl;
}
