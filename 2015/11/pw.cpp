#include <iostream>

using namespace std;


void increment_password(string& password) {
	int digit = password.length() - 1;
	while(digit >= 0) {
		if (++password[digit] > 'z') {
			password[digit] = 'a';
			--digit;
		}
		else {
			return;
		}
	}
	exit(1);
}


bool rule1(const string& password) {
	int straight_len = 1;
	for (int i = 1; i < password.length(); i++) {
		if (password[i] == password[i - 1] + 1) {
			straight_len++;
		}
		else {
			straight_len = 1;
		}
		if (straight_len >= 3) {
			return true;
		}
	}
	return false;
}


bool rule2(const string& password) {
	for (const char ch : password) {
		if (ch == 'i' || ch == 'o' || ch == 'l') {
			return false;
		}
	}
	return true;
}


bool rule3(const string& password) {
	int num_doubles = 0;
	for (int i = 1; i < password.length(); i++) {
		if (password[i] == password[i - 1]) {
			num_doubles++;
			i++;
		}
	}
	return num_doubles >= 2;
}


void next_password(string& password) {
	do {
		increment_password(password);
	} while(!rule1(password) || !rule2(password) || !rule3(password));
}

int main() {
	string password = "hepxcrrq";
	next_password(password);
	cout << password << endl;
	next_password(password);
	cout << password << endl;
}
