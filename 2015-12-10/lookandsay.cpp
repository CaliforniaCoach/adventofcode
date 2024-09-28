#include <iostream>

using namespace std;


string step(string str) {
	char lastchar = str[0];
	int num = 1;
	string result;

	for (int i = 1; i < str.length(); i++) {
		if (str[i] == lastchar) {
			num++;
		}
		else {
			result += '0' + num;
			result += lastchar;
			lastchar = str[i];
			num = 1;
		}
	}
	result += '0' + num;
	result += lastchar;

	return result;
}

int main() {
	string str = "1113222113";
	for (int i = 0; i < 50; i++) {
		str = step(str);
		cout << "Length after " << i + 1 << " steps: " << str.length() << endl;
	}
}
