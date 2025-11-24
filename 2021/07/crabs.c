#include <stdio.h>
#include <limits.h>

#define ABS(x) ((x) < 0 ? -(x) : (x))

int main() {
	int positions[1000];
	int num_positions = 0;
	int pos;
	int min = INT_MAX;
	int max = INT_MIN;
	FILE* fp = fopen("input.txt", "r");
	while (fscanf(fp, "%d", &pos) >= 1) {
		positions[num_positions++] = pos;
		min = (pos < min) ? pos : min;
		max = (pos > max) ? pos : max;
		char chr;
		fscanf(fp, "%c", &chr);
	}
	fclose(fp);

	int min_total_fuel1 = INT_MAX;
	int min_total_fuel2 = INT_MAX;
	for (int mid = min; mid <= max; mid++) {
		int total_fuel1 = 0;
		int total_fuel2 = 0;
		for (int i = 0; i < num_positions; i++) {
			int dist = ABS(positions[i] - mid);
			total_fuel1 += dist;
			total_fuel2 += dist * (dist + 1) / 2;
		}
		if (total_fuel1 < min_total_fuel1) {
			min_total_fuel1 = total_fuel1;
		}
		if (total_fuel2 < min_total_fuel2) {
			min_total_fuel2 = total_fuel2;
		}
	}
	printf("%d\n", min_total_fuel1);
	printf("%d\n", min_total_fuel2);
}
