#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>


int test(int heights[256][256], int basin[256][256], int count, int last_x, int last_y, int x, int y) {
	if (x < 0 || x >= 256 || y < 0 || y >= 256 || basin[y][x]) {
		return count;
	}
	if ((last_x >= 0 && last_y >= 0)) {
		if (heights[y][x] >= 9 || heights[y][x] <= heights[last_y][last_x]) {
			return count;
		}
	}

	basin[y][x] = 1;
	count++;
	count = test(heights, basin, count, x, y, x - 1, y);
	count = test(heights, basin, count, x, y, x + 1, y);
	count = test(heights, basin, count, x, y, x, y - 1);
	count = test(heights, basin, count, x, y, x, y + 1);
	return count;
}

int comp(const void* a, const void* b) {
	return -(*((int*) a) - *((int*) b));
}

int main() {
	int heights[256][256];
	int basin[256][256] = {0};
	for (unsigned int i = 0; i < 256 * 256; i++) {
		*((int*)heights + i) = INT_MAX;
	}
	FILE* fp = fopen("input.txt", "r");
	for (int y = 1; !feof(fp); y++) {
		for (int x = 1; ; x++) {
			int chr = fgetc(fp);
			if (chr >= '0' && chr <= '9') {
				heights[y][x] = chr - '0';
			} else if (chr == '\n' || feof(fp)) {
				break;
			} else {
				assert(0);
			}
		}
	}
	fclose(fp);

	int risk_level_sum = 0;
	int basin_sizes[1000];
	int num_basins = 0;
	for (int y = 1; y < 256 - 1; y++) {
		for (int x = 1; x < 256 - 1; x++) {
			int local_minimum = (
				heights[y][x] < heights[y][x - 1] &&
				heights[y][x] < heights[y][x + 1] &&
				heights[y][x] < heights[y - 1][x] &&
				heights[y][x] < heights[y + 1][x]
			);
			if (local_minimum) {
				risk_level_sum += heights[y][x] + 1;
				basin_sizes[num_basins++] = test(heights, basin, 0, -1, -1, x, y);
			}
		}
	}
	printf("%d\n", risk_level_sum);

	qsort(basin_sizes, num_basins, sizeof(int), comp);
	printf("%ld\n", (long) basin_sizes[0] * basin_sizes[1] * basin_sizes[2]);
}
