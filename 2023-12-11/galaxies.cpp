#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>

using namespace std;


ostream& operator<<(ostream& os, const vector<string>& stringvector) {
	for (string line : stringvector)
		os << line << endl;
	return os;
}

vector<pair<int, int>> galaxy_positions(vector<string> galaxymap, pair<vector<int>,
                                        vector<int>> empty_rows_columns, int expansion) {

	const vector<int>& empty_y = empty_rows_columns.first;
	const vector<int>& empty_x = empty_rows_columns.second;

	const int linelen = galaxymap[0].size();
	vector<int> additional_x(linelen, 0);
	for (int i = 0; i < empty_x.size(); i++)
		for (int x = empty_x[i]; x < (i + 1 < empty_x.size() ? empty_x[i + 1] : linelen); x++)
			additional_x[x] = (i + 1) * expansion;

	vector<pair<int, int>> galaxypos;
	int additional_y = 0;
	int i = 0;
	for (int y = 0; y < galaxymap.size(); y++) {
		if (y == empty_y[i]) {
			additional_y += expansion;
			i++;
		}
		int pos = 0;
		while ((pos = galaxymap[y].find_first_of('#', pos)) != string::npos) {
			galaxypos.push_back(make_pair(pos + additional_x[pos++], y + additional_y));
		}
	}
	return galaxypos;
}


void expand_universe(vector<string>& galaxymap) {
	int linelen = galaxymap[0].size();
	vector<bool> empty(linelen, true);
	for (vector<string>::iterator it = galaxymap.begin(); it != galaxymap.end(); it++) {
		int pos = it->find_first_of('#');
		if (pos == string::npos) {
			it = galaxymap.insert(it, string(linelen, '.'));
			it++;
		} else {
			while (pos != string::npos) {
				empty[pos] = false;
				pos = it->find_first_of('#', pos + 1);
			}
		}
	}
	for (string& line : galaxymap)
		for (int i = linelen - 1; i >= 0; i--)
			if (empty[i])
				line.insert(i, 1, '.');
}

pair<vector<int>, vector<int>> expansion_rows_columns(const vector<string>& galaxymap) {
	vector<int> empty_rows;
	vector<int> empty_columns;
	int linelen = galaxymap[0].size();
	vector<bool> empty(linelen, true);
	for (int y = 0; y < galaxymap.size(); y++) {
		const string& line = galaxymap[y];
		int pos = line.find_first_of('#');
		if (pos == string::npos) {
			empty_rows.push_back(y);
		} else {
			while (pos != string::npos) {
				empty[pos] = false;
				pos = line.find_first_of('#', pos + 1);
			}
		}
	}
	for (int x = 0; x < linelen; x++)
		if (empty[x])
			empty_columns.push_back(x);

	return make_pair(empty_rows, empty_columns);
}

int main() {
	vector<string> galaxymap;
	ifstream file("input.txt");
	string line;
	while (getline(file, line)) {
		galaxymap.push_back(line);
	}

	vector<pair<int, int>> pos = galaxy_positions(galaxymap, expansion_rows_columns(galaxymap), 999999);

	long sum = 0;
	for (int i = 0; i < pos.size(); i++)
		for (int k = i + 1; k < pos.size(); k++)
			sum += abs(pos[i].first - pos[k].first) + abs(pos[i].second - pos[k].second);

	cout << sum << endl;

}
