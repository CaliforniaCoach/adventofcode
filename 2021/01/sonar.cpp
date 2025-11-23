#include <iostream>
#include <fstream>

using namespace std;


int main() {
	ifstream is("input.txt");
	int depth1, depth2, depth3, depth4;
	is >> depth1 >> depth2 >> depth3;
	int moving_sum_current = depth1 + depth2 + depth3;
	int moving_sum_last = moving_sum_current;
	int count_inc1 = (depth2 > depth1 ? 1 : 0) + (depth3 > depth2 ? 1 : 0);
	int count_inc2 = 0;
	while (true) {
		if (!(is >> depth4)) {
			break;
		}
		if (depth4 > depth3) {
			count_inc1++;
		}
		moving_sum_current += depth4 - depth1;
		if (moving_sum_current > moving_sum_last) {
			count_inc2++;
		}
		depth1 = depth2;
		depth2 = depth3;
		depth3 = depth4;
		moving_sum_last = moving_sum_current;
	}
	is.close();
	cout << "Part 1: Increased: " << count_inc1 << endl;
	cout << "Part 2: Increased: " << count_inc2 << endl;
}
