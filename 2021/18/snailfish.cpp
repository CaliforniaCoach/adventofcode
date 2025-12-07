#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cassert>

using namespace std;


class Pair;

class Element {
	public:
		bool isPair;
		int num;
		Pair* pair;

		Element();
		~Element();

		void parse(istream&);
		Pair* check_for_explode(int);
		void add_towards_right(Pair*);
		void add_towards_left(Pair*);
		bool check_for_split();
		long get_magnitude();
};

class Pair {
	public:
		Element left;
		Element right;

		Pair(int num1, int num2) {
			left.isPair = false;
			left.num = num1;
			right.isPair = false;
			right.num = num2;
		};

		Pair(Pair* sn1, Pair* sn2) {
			left.isPair = true;
			left.pair = sn1;
			right.isPair = true;
			right.pair = sn2;
		};

		Pair(const string& str) {
			stringstream sstr(str);
			char chr = sstr.get();
			assert(chr == '[');
			parse(sstr);
		}

		Pair(istream& stream) {
			parse(stream);
		}

		void parse(istream& stream) {
			left.parse(stream);
			char chr;
			stream >> chr;
			assert(chr == ',');
			right.parse(stream);
			stream >> chr;
			assert(chr == ']');
		}

		void reduce() {
			while (true) {
				Pair* pairToExplode;
				while(pairToExplode = check_for_explode(1)) {
					delete pairToExplode;
				}
				if (!check_for_split()) {
					return;
				}
			}
		}

		Pair* check_for_explode(int depth) {
			Pair* pairToExplode = left.check_for_explode(depth);
			if (pairToExplode != NULL) {
				if (pairToExplode->right.num != 0) {
					right.add_towards_right(pairToExplode);
				}
			} else {
				pairToExplode = right.check_for_explode(depth);
				if (pairToExplode != NULL && pairToExplode->left.num != 0) {
					left.add_towards_left(pairToExplode);
				}
			}
			return pairToExplode;
		}

		void add_towards_right(Pair* pairToExplode) {
			left.add_towards_right(pairToExplode);
			if (pairToExplode->right.num != 0) {
				right.add_towards_right(pairToExplode);
			}
		}

		void add_towards_left(Pair* pairToExplode) {
			right.add_towards_left(pairToExplode);
			if (pairToExplode->left.num != 0) {
				left.add_towards_left(pairToExplode);
			}
		}

		bool check_for_split() {
			if (!left.check_for_split()) {
				return right.check_for_split();
			}
			return true;
		}

		long get_magnitude() {
			return 3 * left.get_magnitude() + 2 * right.get_magnitude();
		}
};

Element::Element() {
}

Element::~Element() {
	if (isPair) {
		delete pair;
	}
}

void Element::parse(istream& stream) {
	char chr;
	stream >> chr;
	if (chr == '[') {
		isPair = true;
		pair = new Pair(stream);
	} else {
		isPair = false;
		num = chr - '0';
	}
}

void Element::add_towards_right(Pair* pairToExplode) {
	if (!isPair) {
		num += pairToExplode->right.num;
		// In pairToExplode, denote that adding the right number has already happened
		pairToExplode->right.num = 0;
	} else {
		pair->add_towards_right(pairToExplode);
	}
}

void Element::add_towards_left(Pair* pairToExplode) {
	if (!isPair) {
		num += pairToExplode->left.num;
		// In pairToExplode, denote that adding the left number has already happened
		pairToExplode->left.num = 0;
	} else {
		pair->add_towards_left(pairToExplode);
	}
}

Pair* Element::check_for_explode(int level) {
	if (isPair && level >= 4) {
		isPair = false;
		num = 0;
		return pair;
	}
	return isPair ? pair->check_for_explode(level + 1) : NULL;
}

bool Element::check_for_split() {
	if (isPair) {
		return pair->check_for_split();
	} else if (num >= 10) {
		isPair = true;
		pair = new Pair(num / 2, (num + 1) / 2);
		return true;
	} else {
		return false;
	}
}

long Element::get_magnitude() {
	return isPair ? pair->get_magnitude() : num;
}

ostream& operator<<(ostream&, const Pair&);

ostream& operator<<(ostream& os, const Element& el) {
	if (el.isPair) {
		os << *(el.pair);
	} else {
		os << el.num;
	}
	return os;
}

ostream& operator<<(ostream& os, const Pair& pair) {
	os << '[' << pair.left << ',' << pair.right << ']';
	return os;
}

int main() {
	vector<string> lines;
	ifstream ifs("input.txt");
	string line;
	while (getline(ifs, line)) {
		lines.push_back(line);
	}
	ifs.close();

	Pair* snailfishNumber = NULL;
	for (string& line : lines) {
		if (snailfishNumber == NULL) {
			snailfishNumber = new Pair(line);
		} else {
			snailfishNumber = new Pair(snailfishNumber, new Pair(line));
			snailfishNumber->reduce();
		}
	}
	cout << snailfishNumber->get_magnitude() << endl;
	delete snailfishNumber;

	long maxMagnitude = 0;
	for (int n1 = 0; n1 < lines.size(); n1++) {
		for (int n2 = 0; n2 < lines.size(); n2++) {
			if (n1 != n2) {
				snailfishNumber = new Pair(new Pair(lines[n1]), new Pair(lines[n2]));
				snailfishNumber->reduce();
				maxMagnitude = max(maxMagnitude, snailfishNumber->get_magnitude());
				delete snailfishNumber;
			}
		}
	}
	cout << maxMagnitude << endl;
}
