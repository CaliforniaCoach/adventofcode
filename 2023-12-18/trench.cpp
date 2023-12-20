#include <iostream>
#include <fstream>
#include <set>
#include <vector>
#include <map>

using namespace std;

enum Direction {
	U = 0,
	D = 1,
	R = 2,
	L = 3
};

const map<char, enum Direction> DirMap = {{'U', U}, {'D', D}, {'R', R}, {'L', L}};

class Coord {
	public:
	int x;
	int y;
	Coord operator*(const int multiplier) const {
		return Coord {x * multiplier, y * multiplier};
	};
	Coord operator+(const Coord& rhs) const {
		return Coord {x + rhs.x, y + rhs.y};
	};
	bool operator<(const Coord& rhs) const {
		return x < rhs.x ? x < rhs.x : y < rhs.y;
	};
	Coord& operator+=(const Coord& rhs) {
		x += rhs.x;
		y += rhs.y;
		return *this;
	}
};

const map<enum Direction, Coord> Delta = {
	{U, {0, -1}},
	{D, {0, 1}},
	{R, {1, 0}},
	{L, {-1, 0}}
};

const map<enum Direction, Coord> RightOf = {
	{U, {1, 0}},
	{D, {-1, 0}},
	{R, {0, 1}},
	{L, {0, -1}}
};

const map<enum Direction, Coord> LeftOf = {
	{U, {-1, 0}},
	{D, {1, 0}},
	{R, {0, -1}},
	{L, {0, 1}}
};

struct Instruction {
	enum Direction dir;
	int steps;
	int color;
};

ostream& operator<<(ostream& os, vector<vector<int>> vec) {
	for (vector<int> row : vec) {
		for (int val : row)
			os << (val > 0 ? '#' : '.');
		cout << endl;
	}
	return os;
}

class Trench {
	public:

	vector<vector<int>> field;
	vector<Instruction> plan;
	set<Coord> leftof;
	set<Coord> rightof;

	int min_x = 0;
	int max_x = 0;
	int min_y = 0;
	int max_y = 0;

	int size_x;
	int size_y;

	bool is_inside_leftof = true;
	bool is_inside_rightof = true;

	Trench(string filename) {
		ifstream file(filename);
		char dir;
		int steps;
		string colorcode;
		while (file >> dir >> steps >> colorcode) {
			int color = stoi(colorcode.substr(2, 6), nullptr, 16);
			plan.push_back(Instruction {DirMap.at(dir), steps, color});
		}
		file.close();

		// Find out boundaries of field, in positive and negative x and y directions
		Coord pos = {0, 0};
		for (const Instruction& instruction : plan) {
			pos += Delta.at(instruction.dir) * instruction.steps;
			min_x = min_x < pos.x ? min_x : pos.x;
			min_y = min_y < pos.y ? min_y : pos.y;
			max_x = max_x > pos.x ? max_x : pos.x;
			max_y = max_y > pos.y ? max_y : pos.y;
		}

		// Now create and fill field
		size_x = max_x - min_x + 1;
		size_y = max_y - min_y + 1;
		for (int y = min_y; y <= max_y; y++)
			field.push_back(vector<int>(size_x));

		pos = {-min_x, -min_y};
		for (const Instruction& instruction : plan) {
			for (int i = 0; i < instruction.steps; i++) {
				field[pos.y][pos.x] = instruction.color;
				pos += Delta.at(instruction.dir);
				Coord ptLeft = pos + LeftOf.at(instruction.dir);
				Coord ptRight = pos + RightOf.at(instruction.dir);
				if (ptLeft.x < 0 || ptLeft.x >= size_x || ptLeft.y < 0 || ptLeft.y >= size_y)
					is_inside_leftof = false;
				if (ptRight.x < 0 || ptRight.x >= size_x || ptRight.y < 0 || ptRight.y >= size_y)
					is_inside_rightof = false;
				leftof.insert(ptLeft);
				rightof.insert(ptRight);
			}
		}

		cout << field << endl;

		if (is_inside_leftof)
			fill(leftof, '#');
		if (is_inside_rightof)
			fill(rightof, '#');

		cout << count_tiles() << endl;
	}

	bool fill(set<Coord>& pointList, char fillchar) {
		for (Coord pt : pointList) {
			if (!fill_point(pt, fillchar))
				return false;
		}
		return true;
	}

	bool fill_point(Coord pt, char fillchar) {
		if (pt.y < 0 || pt.y >= size_y)
			return false;
		if (pt.x < 0 || pt.x >= size_y)
			return false;

		if (field[pt.y][pt.x] > 0) {
			return true;
		}

		field[pt.y][pt.x] = fillchar;

		for (pair<int, int> delta : {make_pair(1, 0), make_pair(-1, 0), make_pair(0, 1), make_pair(0, -1)}) {
			if (!fill_point({pt.x + delta.first, pt.y + delta.second}, fillchar))
				return false;
		}

		return true;
	}

	int count_tiles() {
		int count = 0;
		for (vector<int> row : field)
			for (int tile : row)
				if (tile > 0)
					count++;
		return count;
	}
};

int main() {
	Trench trench("input.txt");
}
