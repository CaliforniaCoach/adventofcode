#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <set>

using namespace std;


map<char, pair<pair<int, int>, pair<int, int>>> PIPEELEMENT {
	{'|', make_pair(make_pair(0, -1), make_pair(0, 1))},
	{'-', make_pair(make_pair(-1, 0), make_pair(1, 0))},
	{'L', make_pair(make_pair(0, -1), make_pair(1, 0))},
	{'J', make_pair(make_pair(0, -1), make_pair(-1, 0))},
	{'7', make_pair(make_pair(-1, 0), make_pair(0, 1))},
	{'F', make_pair(make_pair(1, 0), make_pair(0, 1))},
};

map<char, vector<pair<int, int>>> LEFTOF {
	{'|', {make_pair(1, 0)}},
	{'-', {make_pair(0, -1)}},
	{'L', {}},
	{'J', {make_pair(1, 0), make_pair(0, 1)}},
	{'7', {make_pair(0, -1), make_pair(1, 0)}},
	{'F', {}},
};

map<char, vector<pair<int, int>>> RIGHTOF {
	{'|', {make_pair(-1, 0)}},
	{'-', {make_pair(0, 1)}},
	{'L', {make_pair(-1, 0), make_pair(0, 1)}},
	{'J', {}},
	{'7', {}},
	{'F', {make_pair(0, -1), make_pair(-1, 0)}},
};


void print_maze(const vector<string>& maze) {
	for (const string& line : maze)
		cout << line << endl;
}


void print_set(string prefix, const set<pair<int, int>>& set_to_print) {
	cout << prefix << " ";
	for (const pair<int, int>& pt : set_to_print)
		cout << "(" << pt.first << ", " << pt.second << "), ";
	cout << endl << endl;
}

bool is_connected(const vector<string>& maze, int x, int y, int deltax, int deltay) {
	char tile = maze[y + deltay][x + deltax];
	pair<pair<int, int>, pair<int, int>> openings = PIPEELEMENT[tile];
	return (openings.first.first == -deltax && openings.first.second == -deltay) ||
		(openings.second.first == -deltax && openings.second.second == -deltay);
}


int fill_point(vector<string>& maze, int count, int next_x, int next_y, char fillchar) {
	if (next_y < 0 || next_y >= maze.size())
		return -1;
	if (next_x < 0 || next_x >= maze[next_y].size())
		return -1;

	pair<int, int> pt = make_pair(next_x, next_y);
	if (maze[next_y][next_x] == 'X' || maze[next_y][next_x] == fillchar) {
		return count;
	}

	maze[next_y][next_x] = fillchar;

	int currentCount = count + 1;
	for (pair<int, int> delta : {make_pair(1, 0), make_pair(-1, 0), make_pair(0, 1), make_pair(0, -1)}) {
		currentCount = fill_point(maze, currentCount, next_x + delta.first, next_y + delta.second, fillchar);
		if (currentCount < 0)
			return -1;
	}

	return currentCount;
}


int fill(vector<string>& maze, set<pair<int, int>>& pointList, char fillchar) {
	int count = 0;
	for (pair<int, int> pt : set<pair<int, int>>(pointList)) {
		count = fill_point(maze, count, pt.first, pt.second, fillchar);
		if (count < 0)
			return -1;
	}
	return count;
}


int main() {
	vector<string> maze;
	string line;
	ifstream file("input.txt");
	int y = 0;
	pair<int, int> startpos;
	while (getline(file, line)) {
		maze.push_back(line);
		for (int x = 0; x < line.length(); x++) {
			if (line[x] == 'S') {
				startpos = make_pair(x, y);
			}
		}
		y++;
	}
	file.close();

	set<pair<int, int>> points_left_of_path;
	set<pair<int, int>> points_right_of_path;

	int from_x = startpos.first;
	int from_y = startpos.second;
	int cur_x = from_x;
	int cur_y = from_y;
	if (is_connected(maze, startpos.first, startpos.second, 1, 0))
		cur_x++;
	else if (is_connected(maze, startpos.first, startpos.second, -1, 0))
		cur_x--;
	else if (is_connected(maze, startpos.first, startpos.second, 0, 1))
		cur_y++;
	else if (is_connected(maze, startpos.first, startpos.second, 0, -1))
		cur_x--;
	else
		throw;

	int count = 1;
	do {
		char tile = maze[cur_y][cur_x];
		maze[cur_y][cur_x] = 'X';
		pair<pair<int, int>, pair<int, int>> openings = PIPEELEMENT[tile];
		pair<int, int> opening_next;
		vector<pair<int, int>> points_left = LEFTOF[tile];
		vector<pair<int, int>> points_right = RIGHTOF[tile];
		for (pair<int, int>& point: points_left) {
			point.first += cur_x;
			point.second += cur_y;
		}
		for (pair<int, int>& point: points_right) {
			point.first += cur_x;
			point.second += cur_y;
		}
		if (from_x - cur_x == openings.first.first && from_y - cur_y == openings.first.second) {
			opening_next = openings.second;
			points_left_of_path.insert(points_left.begin(), points_left.end());
			points_right_of_path.insert(points_right.begin(), points_right.end());
		}
		else if (from_x - cur_x== openings.second.first && from_y - cur_y == openings.second.second) {
			opening_next = openings.first;
			// We entered the tile "from the wrong direction", therefore swap the points
			// right and left to the path
			points_left_of_path.insert(points_right.begin(), points_right.end());
			points_right_of_path.insert(points_left.begin(), points_left.end());
		}
		else
			throw;

		from_x = cur_x;
		from_y = cur_y;
		cur_x += opening_next.first;
		cur_y += opening_next.second;
		count++;
	} while (maze[cur_y][cur_x] != 'S');
	maze[cur_y][cur_x] = 'X';

	cout << count / 2 << endl;

	int retval;
	if ((retval = fill(maze, points_right_of_path, 'r')) >= 0)
		cout << retval << endl;
	else if ((retval = fill(maze, points_left_of_path, 'l')) >= 0)
		cout << retval << endl;
	else {
		print_set("points left of path: ", points_left_of_path);
		print_set("points right of path: ", points_right_of_path);
	}
	// print_maze(maze);
}
