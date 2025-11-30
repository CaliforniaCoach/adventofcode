#include <stdio.h>
#include <string.h>
#include <sys/resource.h>
#include <time.h>
#include <assert.h>
#include <limits.h>

int maze[1000][1000];
int risk_until_here[1000][1000];
int size_x = 0, size_y = 0;
int min_total_risk = INT_MAX;
time_t starttime;

int go(int x, int y, int risk_so_far) {
	if (x < 0 || y < 0 || x >= size_x || y >= size_y || !x && !y && risk_so_far) {
		return INT_MAX;
	}
	risk_so_far += x | y ? maze[y][x] : 0;
	if (risk_so_far >= min_total_risk) {
		return INT_MAX;
	}
	if (risk_until_here[y][x] > 0 && risk_until_here[y][x] <= risk_so_far) {
		return INT_MAX;
	}
	risk_until_here[y][x] = risk_so_far;

	if (x == size_x - 1 && y == size_y - 1) {
		min_total_risk = risk_so_far;
		return risk_so_far;
	}

	int retval, min_risk = INT_MAX;
	static const int step [4][2] = {{0, 1}, {1, 0}, {-1, 0}, {0, -1}};
	for (int i = 0; i < 4; i++) {
		retval = go(x + step[i][0], y + step[i][1], risk_so_far);
		min_risk = retval < min_risk ? retval : min_risk;
	}
	return min_risk;
}

int find_lowest_risk_route() {
	min_total_risk = INT_MAX;
	memset(risk_until_here, 0, sizeof(risk_until_here));
	starttime = time(NULL);
	int retval = go(0, 0, 0);
	printf("Solution: %d\ntime needed: %ld s\n\n", retval, time(NULL) - starttime);
	return retval;
}

int main() {
	int x = 0;
	FILE* fp = fopen("input.txt", "r");
	while (1) {
		int chr = fgetc(fp);
		if (chr >= '0' && chr <= '9') {
			maze[size_y][x++] = chr - '0';
		} else if (chr == '\n') {
			size_x = x;
			x = 0;
			size_y++;
		} else if (chr == -1) {
			break;
		} else {
			assert(0);
		}
	}
	fclose(fp);

	find_lowest_risk_route();

	for (int tile_x = 0; tile_x < 5; tile_x++) {
		for (int tile_y = tile_x == 0 ? 1 : 0; tile_y < 5; tile_y++) {
			int adder = tile_x + tile_y;
			for (int x = 0; x < size_x; x++) {
				for (int y = 0; y < size_y; y++) {
					int new_risk = (maze[y][x] + adder - 1) % 9 + 1;
					maze[tile_y * size_y + y][tile_x * size_x + x] = new_risk;
				}
			}
		}
	}

	struct rlimit limit;
	int retval = getrlimit(RLIMIT_STACK, &limit);
	assert(retval == 0);
	limit.rlim_cur = limit.rlim_max;
	retval = setrlimit(RLIMIT_STACK, &limit);
	assert(retval == 0);

	size_x *= 5;
	size_y *= 5;
	find_lowest_risk_route();
}
