#include <stdio.h>
#include <string.h>
#include <limits.h>

#define ABS(x) ((x) >= 0 ? (x) : -(x))

#define MAX_LEVELS 1000

char maze[150][150];
int portal_map[150][150] = {0};
int portal_coords[1 << 15][2][2] = {0};
int distmap[MAX_LEVELS][150][150];
int max_y = 0;

int is_outer(int x, int y) {
	return x <= 10 || x >= strlen(maze[y]) - 10 || y <= 10 || y >= max_y - 10;
}

int next(int part, int max_depth, int x, int y, int level, int steps_so_far) {
	if (maze[y][x] != '.' || steps_so_far >= distmap[level][y][x]) {
		return INT_MAX;
	}
	distmap[level][y][x] = steps_so_far;
	if (level == 0 && portal_map[y][x] == ('Z' << 8 | 'Z')) {
		return steps_so_far;
	}

	int portal_code = portal_map[y][x];
	if (portal_code > 0 && portal_code != ('A' << 8 | 'A') && portal_code != ('Z' << 8 | 'Z')) {
		int next_x, next_y;
		if (portal_coords[portal_code][0][0] == x && portal_coords[portal_code][0][1] == y) {
			next_x = portal_coords[portal_code][1][0];
			next_y = portal_coords[portal_code][1][1];
		} else {
			next_x = portal_coords[portal_code][0][0];
			next_y = portal_coords[portal_code][0][1];
		}
		int next_level = level + (part == 1 ? 0 : is_outer(x, y) ? -1 : 1);
		if (next_level >= 0 && next_level <= max_depth) {
			next(part, max_depth, next_x, next_y, next_level, steps_so_far + 1);
		}
	}
	for (int dx = -1; dx <= 1; dx++) {
		for (int dy = -1; dy <= 1; dy++) {
			if (ABS(dx) + ABS(dy) == 1) {
				next(part, max_depth, x + dx, y + dy, level, steps_so_far + 1);
			}
		}
	}
}

int* get_empty_entry(int portal_code) {
	if (portal_coords[portal_code][0][0] == 0) {
		return portal_coords[portal_code][0];
	} else {
		return portal_coords[portal_code][1];
	}
}

void set_portal_coords_horizontal(int x, int y, int portal_code) {
	int* entry = get_empty_entry(portal_code);
	int portal_x = (x > 0 && maze[y][x - 1] == '.') ? x - 1 : x + 2;
	entry[0] = portal_x;
	entry[1] = y;
	portal_map[y][portal_x] = portal_code;
}

void set_portal_coords_vertical(int x, int y, int portal_code) {
	int* entry = get_empty_entry(portal_code);
	int portal_y = (y > 0 && maze[y - 1][x] == '.') ? y - 1 : y + 2;
	entry[0] = x;
	entry[1] = portal_y;
	portal_map[portal_y][x] = portal_code;
}

void init_distmap() {
	for (int y = 0; y < max_y; y++) {
		for (int x = 0; x < strlen(maze[y]); x++) {
			for (int level = 0; level < MAX_LEVELS; level++) {
				distmap[level][y][x] = INT_MAX;
			}
		}
	}
}

int main() {
	FILE* fp = fopen("input.txt", "r");
	while ((fgets(maze[max_y], 150, fp))) {
		max_y++;
	}
	fclose(fp);

	for (int y = 0; y < max_y; y++) {
		for (int x = 0; x < strlen(maze[y]); x++) {
			char chr = maze[y][x];
			if (chr >= 'A' && chr <= 'Z') {
				char chr2 = maze[y][x + 1];
				if (chr2 >= 'A' && chr2 <= 'Z') {
					set_portal_coords_horizontal(x, y, chr << 8 | chr2);
				}
				chr2 = maze[y + 1][x];
				if (chr2 >= 'A' && chr2 <= 'Z') {
					set_portal_coords_vertical(x, y, chr << 8 | chr2);
				}
			}
		}
	}

	int start_x = portal_coords['A' << 8 | 'A'][0][0];
	int start_y = portal_coords['A' << 8 | 'A'][0][1];
	int end_x = portal_coords['Z' << 8 | 'Z'][0][0];
	int end_y = portal_coords['Z' << 8 | 'Z'][0][1];

	init_distmap();
	next(1, 0, start_x, start_y, 0, 0);
	printf("Part 1 solution: %d\n\n", distmap[0][end_y][end_x]);

	int max_depth = 2;
	int steps = INT_MAX;
	while (steps == INT_MAX) {
		init_distmap();
		printf("Checking with max_depth = %d...\n", max_depth);
		next(2, max_depth, start_x, start_y, 0, 0);
		steps = distmap[0][end_y][end_x];
		max_depth++;
	}
	printf("\nPart 2 solution: %d\n", steps);
}
