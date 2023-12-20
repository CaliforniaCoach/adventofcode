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
	long x;
	long y;
	Coord operator*(const long multiplier) const {
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


struct Instruction {
	enum Direction dir;
	long steps;
	int color;
};

ostream& operator<<(ostream& os, vector<vector<long>> vec) {
	for (vector<long> row : vec) {
		for (long val : row)
			os << (val > 0 ? '#' : '.');
		cout << endl;
	}
	return os;
}

string thousand_separators(long long val) {
	long long val2 = val;
	string str = to_string(val);
	int pos = str.length();
	while ((pos -= 3) > 0)
		str.insert(pos, ",");
	return str;
}

class Trench {
	public:

	struct Span {
		long x1;
		long x2;
	};

	vector<Instruction> plan;
	map<long, vector<Span>> rows;

	Trench(string filename) {
		ifstream file(filename);
		char dir;
		long steps;
		string colorcode;
		while (file >> dir >> steps >> colorcode) {
			int color = stoi(colorcode.substr(2, 6), nullptr, 16);
			plan.push_back(Instruction {DirMap.at(dir), steps, color});
		}
		file.close();

		translate_normal();
		process_plan();

		rows.clear();
		translate_special();
		process_plan();
	}

	void translate_normal() {
		Coord pos = {0, 0};
		for (const Instruction& instruction : plan) {
			if (instruction.dir == U) {
				pos.y -= instruction.steps;
			}
			else if (instruction.dir == D) {
				pos.y += instruction.steps;
			}
			else {
				Span span;
				if (instruction.dir == R) {
					span = {pos.x, pos.x + instruction.steps};
					pos.x += instruction.steps;
				} else if (instruction.dir == L) {
					span = {pos.x - instruction.steps, pos.x};
					pos.x -= instruction.steps;
				} else {
					throw;
				}
				pair<map<long, vector<Span>>::iterator, bool> retval;
				retval = rows.insert(make_pair(pos.y, vector<Span>()));
				retval.first->second.push_back(span);
			}
		}
		if (pos.x != 0 || pos.y != 0)
			throw;

	}

	void translate_special() {
		Coord pos = {0, 0};
		Direction last_direction = L;
		bool first_move = true;
		for (const Instruction& entry : plan) {

			// Special "translation"
			Instruction instruction;
			int dir = entry.color & 0x0f;
			instruction.dir = dir == 0 ? R : dir == 1 ? D : dir == 2 ? L : U;
			instruction.steps = entry.color >> 4;

			if (instruction.dir == U) {
				if (!first_move && (last_direction == U || last_direction == D))
					throw;
				pos.y -= instruction.steps;
			}
			else if (instruction.dir == D) {
				if (!first_move && (last_direction == U || last_direction == D))
					throw;
				pos.y += instruction.steps;
			}
			else {
				Span span;
				if (instruction.dir == R) {
					if (!first_move && (last_direction == R || last_direction == L))
						throw;
					span = {pos.x, pos.x + instruction.steps};
					pos.x += instruction.steps;
				} else if (instruction.dir == L) {
					if (!first_move && (last_direction == R || last_direction == L))
						throw;
					span = {pos.x - instruction.steps, pos.x};
					pos.x -= instruction.steps;
				} else {
					throw;
				}
				pair<map<long, vector<Span>>::iterator, bool> retval;
				retval = rows.insert(make_pair(pos.y, vector<Span>()));
				retval.first->second.push_back(span);
			}
			last_direction = instruction.dir;
			first_move = false;
		}
		if (pos.x != 0 || pos.y != 0)
			throw;

	}

	void process_plan() {
		long long area = 0LL;
		auto rowIt = rows.begin();
		long last_y = rowIt->first;
		vector<Span> curSpans = rowIt->second;
		long total_span = 0;
		for (const Span& curSpan : curSpans) {
			total_span += curSpan.x2 - curSpan.x1 + 1;
		}
		area = (long long)total_span;
		rowIt++;
		while (rowIt != rows.end()) {
			long y = rowIt->first;
			vector<Span>& spans = rowIt->second;
			// First: Calculate area over spans
			long total_span = 0;
			for (const Span& curSpan : curSpans) {
				total_span += curSpan.x2 - curSpan.x1 + 1;
			}
			area += (long long)total_span * (long long)(y - last_y);
			// Second: Extend or shrink existing spans
			vector<Span>::iterator it1 = curSpans.begin();
			while(it1 != curSpans.end()) {
				Span& curSpan = *it1;
				bool proceed = true;
				vector<Span>::iterator it2 = spans.begin();
				while (it2 != spans.end()) {
					const Span span = *it2;
					if (curSpan.x1 == span.x1 && curSpan.x2 == span.x2) {
						it1 = curSpans.erase(it1);
						spans.erase(it2);
						proceed = false;
						break;
					}
					else if (curSpan.x1 == span.x2) {
						curSpan.x1 = span.x1;
						area += span.x2 - span.x1;
					}
					else if (curSpan.x2 == span.x1) {
						curSpan.x2 = span.x2;
						area += span.x2 - span.x1;
					}
					else if (curSpan.x1 == span.x1) {
						curSpan.x1 = span.x2;
					}
					else if (curSpan.x2 == span.x2) {
						curSpan.x2 = span.x1;
					}
					it2++;
				}
				if (proceed) {
					it1++;
				}
			}
			// Third: Split spans
			for (vector<Span>::iterator it = curSpans.begin(); it != curSpans.end(); it++) {
				Span& curSpan = *it;
				for (const Span& span : spans) {
					if (span.x1 > curSpan.x1 && span.x2 < curSpan.x2) {
						Span newSpan {curSpan.x1, span.x1};
						curSpan.x1 = span.x2;
						it = curSpans.insert(it, newSpan);
					}
				}
			}
			// Fourth: Insert new spans completely outside of current spans
			vector<Span> new_spans;
			for (const Span& span : spans) {
				bool span_outside = true;
				for (const Span& curSpan : curSpans) {
					if (!(span.x2 < curSpan.x1 || span.x1 > curSpan.x2)) {
						span_outside = false;
						break;
					}
				}
				if (span_outside) {
					new_spans.push_back(span);
				}
			}
			for (const Span& new_span : new_spans) {
				curSpans.push_back(new_span);
				area += new_span.x2 - new_span.x1 + 1;
			}
			// Fifth: Merge overlapping spans
			for (vector<Span>::iterator it1 = curSpans.begin(); it1 != curSpans.end(); it1++) {
				vector<Span>::iterator it2 = it1 + 1;
				while (it2 != curSpans.end()) {
					bool proceed = true;
					if (it1->x2 >= it2->x1 && it1->x2 <= it2->x2) {
						area -= it1->x2 - it2->x1 + 1;
						it1->x2 = it2->x2;
						it2 = curSpans.erase(it2);
						proceed = false;
					} else if (it1->x1 >= it2->x1 && it1->x1 <= it2->x2) {
						area -= it2->x2 - it1->x1 + 1;
						it1->x1 = it2->x1;
						it2 = curSpans.erase(it2);
						proceed = false;
					}
					if (proceed) {
						it2++;
					}
				}
			}
			last_y = y;
			rowIt++;
		}
		if(curSpans.size() > 0)
			throw;

		cout << thousand_separators(area) << " = " << area << endl;
	}
};

int main() {
	Trench trench("input.txt");
}
