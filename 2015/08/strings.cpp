#include <iostream>
#include <fstream>

using namespace std;

int main() {
	ifstream file("input.txt");
	string line;

	int memory = 0;
	int literal = 0;
	int encoded = 0;

	while (getline(file, line)) {
		literal += line.length();
		for (int i = 1; i < line.length() - 1; i++) {
			if (line[i] == '\\') {
				if (line[++i] == 'x') {
					i += 2;
				}
			}
			memory++;
		}

		encoded += 2;
		for (char ch : line) {
			switch(ch) {
				case '"':
				case '\\':
					encoded += 2;
					break;

				default:
					encoded++;
			}
		}
	}

	cout << literal - memory << endl;
	cout << encoded - literal << endl;
}
