#include <iostream>
#include <fstream>
#include <map>
#include <set>

using namespace std;


class Cave {
	public:
		bool big;
		bool end;
		int visited;
		set<Cave*> exits;

		int hop(const map<string, Cave>& caves, bool one_visited_twice) {
			if (this->end) {
				return 1;
			}
			if (!big && visited) {
				if (one_visited_twice) {
					return 0;
				} else {
					one_visited_twice = true;
				}
			}

			int num_paths = 0;
			visited++;
			for (Cave* cave : exits) {
				num_paths += cave->hop(caves, one_visited_twice);
			}
			visited--;

			return num_paths;
		}
};

Cave& create_or_get_cave(map<string, Cave>& caves, string name) {
	map<string, Cave>::iterator retval = caves.find(name);
	if (retval == caves.end()) {
		Cave cave = Cave();
		cave.big = name[0] >= 'A' && name[0] <= 'Z';
		cave.end = name == "end";
		caves.insert({name, cave});
	}
	return caves.at(name);
}

int main() {
	map<string, Cave> caves;
	ifstream ifs("input.txt");
	string line;
	while (getline(ifs, line)) {
		string name1 = line.substr(0, line.find('-'));
		string name2 = line.substr(line.find('-') + 1);
		if (name2 != "start") {
			create_or_get_cave(caves, name1).exits.insert(&create_or_get_cave(caves, name2));
		}
		if (name1 != "start") {
			create_or_get_cave(caves, name2).exits.insert(&create_or_get_cave(caves, name1));
		}
	}
	ifs.close();

	cout << caves["start"].hop(caves, true) << endl;
	cout << caves["start"].hop(caves, false) << endl;
}
