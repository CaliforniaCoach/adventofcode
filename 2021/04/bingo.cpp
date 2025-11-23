#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <array>
#include <iomanip>
#include <cassert>

using namespace std;


ostream& operator<<(ostream& os, const vector<int>& vec) {
	for (int x : vec)
		os << x << ",";
	return os;
}

class Board {
	public:
		array<array<int, 5>, 5> numbers;
		array<array<bool, 5>, 5> drawn = {};
		bool won_already = false;

		bool play_number(int number) {
			bool won = false;
			for (int row = 0; row < numbers.size(); row++) {
				for (int col = 0; col < numbers[row].size(); col++) {
					if (numbers[row][col] == number) {
						drawn[row][col] = true;
						bool row_complete = true;
						bool col_complete = true;
						for (int i = 0; i < drawn.size(); i++) {
							if (!drawn[i][col]) {
								col_complete = false;
							}
							if (!drawn[row][i]) {
								row_complete = false;
							}
						}
						if (row_complete || col_complete) {
							won = true;
						}
					}
				}
			}
			won_already = won_already || won;
			return won;
		}

		int score() {
			int score = 0;
			for (int row = 0; row < numbers.size(); row++) {
				for (int col = 0; col < numbers[row].size(); col++) {
					if (!drawn[row][col]) {
						score += numbers[row][col];
					}
				}
			}
			return score;
		}
};

istream& operator>>(istream& is, Board &board) {
	for (array<int, 5> &row : board.numbers) {
		for (int &number : row) {
			if (!(is >> number)) {
				return is;
			}
		}
	}
	return is;
}

ostream& operator<<(ostream& os, const Board& board) {
	for (const array<int, 5> &row : board.numbers) {
		for (const int &number : row) {
			os << setw(4) << number;
		}
		os << endl;
	}
	return os;
}


int main() {
	vector<int> numbers;
	vector<Board> boards;
	int number;
	string line;
	char chr;

	ifstream ifs("input.txt");
	getline(ifs, line);
	stringstream sts(line);
	while (sts >> number) {
		numbers.push_back(number);
		sts >> chr;
	}
	Board board;
	while (ifs >> board) {
		boards.push_back(board);
	}
	ifs.close();

	bool one_board_won = false;
	for (int number : numbers) {
		vector<Board*> boards_won_this_round;
		bool all_boards_won = true;
		for (Board& board : boards) {
			if (!board.won_already && board.play_number(number)) {
				boards_won_this_round.push_back(&board);
			}
			all_boards_won = all_boards_won && board.won_already;
		}
		if (!one_board_won && !boards_won_this_round.empty()) {
			assert(boards_won_this_round.size() == 1);
			cout << number * boards_won_this_round[0]->score() << endl;
			one_board_won = true;
		}
		if (all_boards_won) {
			assert(boards_won_this_round.size() == 1);
			cout << number * boards_won_this_round[0]->score() << endl;
			break;
		}
	}
}
