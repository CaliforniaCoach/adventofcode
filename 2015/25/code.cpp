#include <iostream>

using namespace std;


const int ROW = 3010;
const int COL = 3019;

const long FIRST_CODE = 20151125L;
const long MULT = 252533L;
const long DIV = 33554393L;


inline long sum_up_to(int n) {
	// sum(1 + 2 + 3 + ... + n) = (n + 1) * n / 2
	return (n + 1) * n / 2;
}

inline long num_col_1_of_row(int n) {
	return 1 + sum_up_to(n - 1);
}

inline long num(int row, int col) {
	int num_diag = row + col - 1;
	return num_col_1_of_row(num_diag) + (col - 1);
}

int main() {
	// Test
	for (int row = 1; row <= 6; row++) {
		for (int col = 1; col <= 6; col++)
			cout << "row " << row << ", col " << col << ": " << num(row, col) << endl;
	}

	long code = FIRST_CODE;
	long n_th_code = num(ROW, COL);
	for (long i = 0L; i < n_th_code - 1; i++) {
		code = code * MULT % DIV;
	}

	cout << endl << "Code: " << code << endl;
}
