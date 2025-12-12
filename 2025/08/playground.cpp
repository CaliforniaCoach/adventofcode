#include <iostream>
#include <fstream>
#include <array>
#include <vector>
#include <set>
#include <cmath>
#include <algorithm>
#include <cassert>

using namespace std;


class Box;

struct Circuit {
	Circuit(Box* initial_box) {
		boxes.push_back(initial_box);
	}
	vector<Box*> boxes;
};

struct Box {
	Box(int x, int y, int z) : pos({x, y, z}) {
		circuit = new Circuit(this);
	};
	array<int, 3> pos;
	Circuit* circuit;
	int& operator[](int i) {
		return pos[i];
	}
};

struct Connection {
	Box* box1;
	Box* box2;
	double dist;
	Connection(Box* box1, Box* box2) : box1(box1), box2(box2) {
		double dx = (*box1)[0] - (*box2)[0];
		double dy = (*box1)[1] - (*box2)[1];
		double dz = (*box1)[2] - (*box2)[2];
		dist = sqrt(dx * dx + dy * dy + dz * dz);
	};
	int operator<(const Connection& conn2) const {
		return dist < conn2.dist;
	}
	bool connect() {
		if (box1->circuit != box2->circuit) {
			Circuit* obsolete_circuit = box2->circuit;
			for (Box* box : box2->circuit->boxes) {
				box->circuit = box1->circuit;
				box->circuit->boxes.push_back(box);
			}
			delete obsolete_circuit;
			return true;
		}
		return false;
	}
};

int main() {
	vector<Box*> boxes;
	ifstream ifs("input.txt");
	char chr;
	int x, y, z;
	while (ifs >> x >> chr >> y >> chr >> z) {
		boxes.push_back(new Box(x, y, z));
	}
	ifs.close();

	vector<Connection> connections;
	for (int i = 0; i < boxes.size(); i++) {
		for (int k = i + 1; k < boxes.size(); k++) {
			connections.emplace_back(boxes[i], boxes[k]);
		}
	}
	sort(connections.begin(), connections.end());

	int num_circuits = boxes.size();
	for (int i = 0; ; i++) {
		num_circuits -= connections[i].connect() ? 1 : 0;
		if (i == 1000 - 1) {
			set<Circuit*> circuits;
			vector<long> circuit_sizes;
			for (Box* box : boxes) {
				if(circuits.insert(box->circuit).second) {
					circuit_sizes.push_back(box->circuit->boxes.size());
				}
			}
			assert(circuit_sizes.size() == num_circuits);
			sort(circuit_sizes.begin(), circuit_sizes.end());
			reverse(circuit_sizes.begin(), circuit_sizes.end());
			cout << circuit_sizes[0] * circuit_sizes[1] * circuit_sizes[2] << endl;
		}
		if (num_circuits == 1) {
			cout << (long) (*connections[i].box1)[0] * (*connections[i].box2)[0] << endl;
			break;
		}
	}
	delete boxes[0]->circuit;
	for (Box* box : boxes) {
		delete box;
	}
}
