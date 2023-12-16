#include <iostream>
#include <vector>

using namespace std;

ostream& operator<<(ostream& os, vector<int> intvec) {
	for (int num : intvec)
		cout << num << endl;
}

int main() {
	vector<int> vec {1, 2, 3};

	cout << vec;
}
