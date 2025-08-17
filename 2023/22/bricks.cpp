#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <set>

using namespace std;


inline int min(int a, int b) {
	return a < b ? a : b;
}

inline int max(int a, int b) {
	return a > b ? a : b;
}

inline bool overlaps(int x1a, int x1b, int x2a, int x2b) {
	return min(x1a, x1b) <= max(x2a, x2b) && max(x1a, x1b) >= min(x2a, x2b);
}

struct Cube {
	int x = -1;
	int y = -1;
	int z = -1;
};

struct Brick {
	Cube c1;
	Cube c2;

	vector<Brick*> supports;
	vector<Brick*> supported_by;

	int lower_z() const {
		return min(c1.z, c2.z);
	}

	bool operator<(const Brick& rhs) const {
		return lower_z() < rhs.lower_z();
	}

	bool operator<=(const Brick& rhs) const {
		return lower_z() <= rhs.lower_z();
	}

	bool interferes_with(const Brick& rhs) {
		return overlaps(c1.x, c2.x, rhs.c1.x, rhs.c2.x) && overlaps(c1.y, c2.y, rhs.c1.y, rhs.c2.y);
	}

	int drop(vector<Brick>& bricks) {
		int drop_to_z = 1;
		for (Brick& brick : bricks) {
			if (this == &brick) {
				break;
			}
			if (interferes_with(brick)) {
				int new_z = max(brick.c1.z, brick.c2.z) + 1;
				if (new_z > drop_to_z) {
					supported_by.clear();
					drop_to_z = new_z;
				}
				if (new_z == drop_to_z) {
					supported_by.push_back(&brick);
				}
			}
		}
		int drop_z = lower_z() - drop_to_z;
		if (drop_z < 0)
			throw;
		c1.z -= drop_z;
		c2.z -= drop_z;
		for (Brick* brick : supported_by) {
			brick->supports.push_back(this);
		}
		return drop_z;
	}

	string to_string() {
		return std::to_string(c1.x) + "," + std::to_string(c1.y) + "," + std::to_string(c1.z) + " ~ " + std::to_string(c2.x) + "," + std::to_string(c2.y) + "," + std::to_string(c2.z);
	}
};

bool contains(const set<const Brick*>& container, const Brick& brick) {
	return find(container.begin(), container.end(), &brick) != container.end();
}

set<const Brick*>& bricks_that_would_fall(set<const Brick*>& falling_bricks, const Brick& brick) {
	falling_bricks.insert(&brick);
	for (const Brick* supported_brick : brick.supports) {
		bool supported_brick_will_fall = true;
		for (const Brick* supported_brick_supported_by : supported_brick->supported_by) {
			if (!contains(falling_bricks, *supported_brick_supported_by)) {
				supported_brick_will_fall = false;
				break;
			}
		}
		if (supported_brick_will_fall) {
			bricks_that_would_fall(falling_bricks, *supported_brick);
		}
	}
	return falling_bricks;
}

int main() {
	vector<Brick> bricks;
	ifstream file("input.txt");
	Brick b;
	char c;
	while (file >> b.c1.x >> c >> b.c1.y >> c >> b.c1.z >> c >> b.c2.x >> c >> b.c2.y >> c >> b.c2.z) {
		bricks.push_back(b);
	}
	file.close();

	int total_dropped;
	do {
		total_dropped = 0;
		sort(bricks.begin(), bricks.end());
		for (Brick& brick : bricks) {
			brick.supports.clear();
			brick.supported_by.clear();
		}
		for (Brick& brick : bricks) {
			total_dropped += brick.drop(bricks);
		}

		int count = 0;
		for (const Brick& brick : bricks) {
			bool can_be_disintegrated = true;
			for (Brick* supported_brick : brick.supports) {
				if (max(brick.c1.z, brick.c2.z) != min(supported_brick->c1.z, supported_brick->c2.z) - 1) {
					throw;
				}
				if (supported_brick->supported_by.size() == 1) {
					can_be_disintegrated = false;
					break;
				}
				if (supported_brick->supported_by.empty()) {
					throw;
				}
			}
			if (can_be_disintegrated) {
				count++;
			}
		}

		cout << count << endl;
	} while (total_dropped != 0);

	int count = 0;
	for (const Brick& brick : bricks) {
		set<const Brick*> falling_bricks;
		count += bricks_that_would_fall(falling_bricks, brick).size() - 1;
	}
	cout << count << endl;
}
