#include <iostream>
#include <fstream>
#include <string>

using namespace std;


int main() {
	long long invalid_sum1 = 0;
	long long invalid_sum2 = 0;
	ifstream ifs("input.txt");
	long from, to;
	char separator1, separator2;
	while (ifs >> from >> separator1 >> to) {
		ifs >> separator2;
		for (long id = from; id <= to; id++) {
			string id_string = to_string(id);
			for (int pattern_len = id_string.length() / 2; pattern_len >= 1; pattern_len--) {
				string pattern = id_string.substr(0, pattern_len);
				bool invalid = true;
				for (int i = 1; i * pattern_len < id_string.length(); i++) {
					if (id_string.substr(i * pattern_len, pattern_len) != pattern) {
						invalid = false;
						break;
					}
				}
				if (invalid) {
					if (pattern_len * 2 == id_string.length()) {
						invalid_sum1 += id;
					}
					invalid_sum2 += id;
					break;
				}
			}
		}
	}
	ifs.close();
	cout << invalid_sum1 << endl;
	cout << invalid_sum2 << endl;
}
