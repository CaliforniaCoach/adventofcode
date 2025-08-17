#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <algorithm>

using namespace std;


int compose(const vector<array<int, 5>>& ingredients, vector<int>& recipe, int total_used_so_far,
			int ing_index, int maximum, int target_calories) {
	
	if (ing_index >= 5) {
		if (target_calories > 0) {
			int calories = 0;
			for (int k = 0; k < ingredients.size() - 1; k++) {
				calories += ingredients[k].back() * recipe[k];
			}
			calories += ingredients.back().back() * (100 - total_used_so_far);
			if (calories != target_calories) {
				return maximum;
			}
		}
		int score = 1;
		for (int i = 0; i < 4; i++) {
			int property = 0;
			for (int k = 0; k < ingredients.size() - 1; k++) {
				property += ingredients[k][i] * recipe[k];
			}
			property += ingredients.back()[i] * (100 - total_used_so_far);
			if (property <= 0) {
				return maximum;
			}
			score *= property;
		}
		return max(maximum, score);
	}

	for (int rest = 0; rest < 100 - total_used_so_far; rest++) {
		recipe[ing_index] = rest;
		maximum = compose(ingredients, recipe, total_used_so_far + rest, ing_index + 1, maximum, target_calories);
	}

	return maximum;
}


int main() {
	ifstream file("input.txt");
	vector<array<int, 5>> ingredients;
	while (!file.eof()) {
		int cap, dur, flav, text, cal;
		string dummy;
		file >> dummy >> dummy >> cap >> dummy >> dummy >> dur >> dummy >> dummy >> flav >> dummy;
		file >> dummy >> text >> dummy >> dummy >> cal >> ws;
		ingredients.push_back({cap, dur, flav, text, cal});
	}
	file.close();

	vector<int> recipe(ingredients.size() - 1, 0);

	cout << compose(ingredients, recipe, 0, 0, 0, -1) << endl;

	cout << compose(ingredients, recipe, 0, 0, 0, 500) << endl;
}
