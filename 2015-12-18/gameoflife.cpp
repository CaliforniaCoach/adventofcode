#include <iostream>
#include <fstream>
#include <array>

using namespace std;


typedef array<array<bool, 100>, 100> lightarray;


bool get_light(const lightarray& lights, int x, int y) {
	if (y >= 0 && y < lights.size() && x >= 0 && x < lights[y].size()) {
		return lights[y][x];
	}
	else {
		return false;
	}
}


void step(lightarray& lights) {
	lightarray new_lights;
	for (int y = 0; y < 100; y++) {
		for (int x = 0; x < 100; x++) {
			int neighbors = 0;
			for (int testx = -1; testx <= 1; testx++) {
				for (int testy = -1; testy <= 1; testy++) {
					if (testx != 0 || testy != 0) {
						if (get_light(lights, x + testx, y + testy)) {
							neighbors++;
						}
					}
				}
			}
			new_lights[y][x] = (neighbors == 3 || (lights[y][x] && neighbors == 2));
		}
	}
	lights = new_lights;
}


int count(const lightarray& lights) {
	int count = 0;
	for (int y = 0; y < lights.size(); y++) {
		for (int x = 0; x < lights[y].size(); x++) {
			if (lights[y][x]) {
				count++;
			}
		}
	}
	return count;
}


void turn_on_corner_lights(lightarray& lights) {
	lights[0][0] = true;
	lights[lights.size() - 1][0] = true;
	lights[0][lights[0].size() - 1] = true;
	lights[lights.size() - 1][lights[0].size() - 1] = true;
}


int main() {
	lightarray lights1;
	ifstream file("input.txt");
	for (int y = 0; y < 100; y++) {
		for (int x = 0; x < 100; x++) {
			char ch;
			file >> ch;
			if (ch == '#') {
				lights1[y][x] = true;
			}
			else if (ch == '.') {
				lights1[y][x] = false;
			}
			else {
				throw runtime_error("Unexpected character");
			}
		}
		file >> ws;
	}
	file.close();

	lightarray lights2 = lights1;
	turn_on_corner_lights(lights2);
	for (int i = 0; i < 100; i++) {
		step(lights1);
		step(lights2);
		turn_on_corner_lights(lights2);
	}

	cout << count(lights1) << endl;
	cout << count(lights2) << endl;
}
