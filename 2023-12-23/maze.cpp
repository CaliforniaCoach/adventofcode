#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;


enum Direction {
	U, D, L, R
};

struct Point {
	int x;
	int y;

	bool operator==(const Point& pt2) const {
		return x == pt2.x && y == pt2.y;
	}

	bool operator<(const Point& pt2) const {
		return x < pt2.x || (x == pt2.x && y < pt2.y);
	}
};

struct Alley {
	int len = 0;
	int direction;
	Point begin = {-1, -1};
	Point end = {-1, -1};

	vector<Point> path;
};

Alley* crawl(const vector<string>& maze, vector<Alley*>& alleys, map<Point, vector<Alley*>*>& crossings, int x, int y, Direction dir) {
	Alley* alley = new Alley();
	alleys.push_back(alley);

	char sym_forward[] = {'^', 'v', '<', '>'};
	char sym_backward[] = {'v', '^', '>', '<'};
	int new_dir = maze[y][x] == sym_forward[dir] ? 1 : maze[y][x] == sym_backward[dir] ? -1 : maze[y][x] == '.' ? 0 : -999;
	if (new_dir != 0) {
		if (new_dir * alley->direction == -1)
			alley->direction = -2;
		else if(alley->direction > -2)
			alley->direction = new_dir;
	}
	if (new_dir < -1)
		throw;
	
	int len = 0;
	while (true) {
		int left = 0, right = 0, down = 0, up = 0;
		if (maze[y][x - 1] != '#')
			left = 1;
		if (maze[y][x + 1] != '#')
			right = 1;
		if (maze[y + 1][x] != '#')
			down = 1;
		if (y > 0 && maze[y - 1][x] != '#')
			up = 1;
		
		alley->len++;
		alley->path.push_back({x, y});

		if (left + right + down + up == (y > 0 ? 2 : 1)) {
			int new_dir;
			if (left && dir != R) {
				x--;
				dir = L;
			}
			else if (right && dir != L) {
				x++;
				dir = R;
			}
			else if (up && dir != D) {
				y--;
				dir = U;
			}
			else if (down && dir != U) {
				y++;
				dir = D;
			}
			
			if (y == maze.size() - 1) {
				alley->end = {x, y};
				return alley;
			}
		}
		else if (left + right + down + up == 3 || left + right + down + up == 4) {
			if (maze[y][x] != '.')
				throw;
			Point pt {x, y};
			alley->end = pt;
			if (crossings.find(pt) == crossings.end()) {
				vector<Alley*>* crosspoint_alleys = new vector<Alley*>(1, alley);
				crossings.insert(make_pair(pt, crosspoint_alleys));
				if (left && dir != R) {
					Alley* new_alley = crawl(maze, alleys, crossings, x - 1, y, L);
					crosspoint_alleys->push_back(new_alley);
					new_alley->begin = pt;
				}
				if (right && dir != L) {
					Alley* new_alley = crawl(maze, alleys, crossings, x + 1, y, R);
					crosspoint_alleys->push_back(new_alley);
					new_alley->begin = pt;
				}
				if (up && dir != D) {
					Alley* new_alley = crawl(maze, alleys, crossings, x, y - 1, U);
					crosspoint_alleys->push_back(new_alley);
					new_alley->begin = pt;
				}
				if (down && dir != U) {
					Alley* new_alley = crawl(maze, alleys, crossings, x, y + 1, D);
					crosspoint_alleys->push_back(new_alley);
					new_alley->begin = pt;
				}
			}
			break;
		}
		else
			throw;
	}
	return alley;
}

void print_maze(vector<string> maze, const vector<const Alley*>& alleys) {
	for (const Alley* alley : alleys) {
		for (Point pt : alley->path) {
			maze[pt.y][pt.x] = '0';
		}
	}
	for (string line : maze) {
		for (char chr : line) {
			cout << (chr == '#' ? '#' : chr == '0' ? '.' : ' ');
		}
		cout << endl;
	}
}

int path(const vector<string>& maze, const Alley& alley, int dir, const map<Point, vector<Alley*>*>& crossings, vector<const Alley*>& alleys_visited, vector<Point>& crossings_visited, int len_so_far, bool slip = true) {
	len_so_far += alley.len;
	Point endpoint = dir > 0 ? alley.end : alley.begin;
	vector<Point>::iterator it = find(crossings_visited.begin(), crossings_visited.end(), endpoint);
	if (it == crossings_visited.end()) {
		map<Point, vector<Alley*>*>::const_iterator it2 = crossings.find(endpoint);
		if (it2 == crossings.end()) {
			if (len_so_far > 6450) {
				print_maze(maze, alleys_visited);
				cout << len_so_far << endl << endl;
			}
			return len_so_far;
		} else {
			crossings_visited.push_back(endpoint);
			alleys_visited.push_back(&alley);
			int max_len = 0;
			for (Alley* next_alley : *it2->second) {
				if (next_alley != &alley && find(alleys_visited.begin(), alleys_visited.end(), next_alley) == alleys_visited.end()) {
					int dir = next_alley->begin == endpoint ? 1 : -1;
					if (!slip || next_alley->direction == 0 || next_alley->direction == dir) {
						int new_len = path(maze, *next_alley, dir, crossings, alleys_visited, crossings_visited, len_so_far, slip);
						max_len = new_len > max_len ? new_len : max_len;
					}
				}
			}
			alleys_visited.pop_back();
			crossings_visited.pop_back();
			return max_len;
		}
	}
	return 0;
}

int main() {
	vector<string> maze;
	string line;
	ifstream file("input.txt");
	while (getline(file, line))
		maze.push_back(line);
	file.close();

	vector<Alley*> alleys;
	map<Point, vector<Alley*>*> crossings;
	int start_x = maze[0].find(".");
	Alley* start_alley = crawl(maze, alleys, crossings, start_x, 0, D);
	start_alley->begin = {start_x, 0};

	vector<const Alley*> alleys_visited;
	vector<Point> crossings_visited;
	int max_len = path(maze, *start_alley, 1, crossings, alleys_visited, crossings_visited, 0);
	cout << max_len << endl;

	max_len = path(maze, *start_alley, 1, crossings, alleys_visited, crossings_visited, 0, false);
	cout << max_len << endl;
}
