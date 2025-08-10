#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <assert.h>
#include <time.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define ABS(a) ((a) < 0 ? -(a) : (a))

int get_dist_by_keymap(unsigned long distmap[100][100][10000], unsigned long keys, int x, int y) {
	unsigned long* distlist = distmap[y][x];
	for (int i = 0; distlist[i] != 0; i += 2) {
		assert(i < 10000);
		if ((distlist[i] == keys)) {
			return distlist[i + 1];
		}
	}
	return INT_MAX;
}

void put_dist_by_keymap(unsigned long distmap[100][100][10000], unsigned long keys, int x, int y,
						unsigned long dist) {
	unsigned long* distlist = distmap[y][x];
	int i;
	for (i = 0; distlist[i] != 0; i += 2) {
		if (distlist[i] == keys) {
			distlist[i + 1] = dist;
			return;
		}
	}
	assert(i < 10000);
	distlist[i] = keys;
	distlist[i + 1] = dist;
}


int next_step(char maze[100][100], unsigned long distmap[100][100][10000], unsigned long keys,
			  int num_keys, int x, int y, int dx, int dy, unsigned long steps) {
	x += dx;
	y += dy;
	char chr = maze[y][x];
	if (chr == '#') {
		return INT_MAX;
	}
	if (chr >= 'A' && chr <= 'Z' && !((keys >> (chr - 'A')) & 1)) {
		return INT_MAX;
	}
	if (chr >= 'a' && chr <= 'z' && !((keys >> (chr - 'a')) & 1)) {
		keys |= 1 << (chr - 'a');
		num_keys--;
		if (num_keys == 0) {
			return steps;
		}
	}
	if (get_dist_by_keymap(distmap, keys, x, y) <= steps) {
		return INT_MAX;
	}
	put_dist_by_keymap(distmap, keys, x, y, steps);
	int min_steps = INT_MAX;
	if ((dx || dy) && !(chr >= 'a' && chr <= 'z')) {
		int new_steps = next_step(maze, distmap, keys, num_keys, x, y, dx, dy, steps + 1);
		min_steps = MIN(min_steps, new_steps);
		if (dx) {
			new_steps = next_step(maze, distmap, keys, num_keys, x, y, 0, -1, steps + 1);
			min_steps = MIN(min_steps, new_steps);
			new_steps = next_step(maze, distmap,keys, num_keys, x, y, 0, 1, steps + 1);
			min_steps = MIN(min_steps, new_steps);
		} else {
			new_steps = next_step(maze, distmap, keys, num_keys, x, y, -1, 0, steps + 1);
			min_steps = MIN(min_steps, new_steps);
			new_steps = next_step(maze, distmap, keys, num_keys, x, y, 1, 0, steps + 1);
			min_steps = MIN(min_steps, new_steps);
		}
	} else {
		for (int dx = -1; dx <= 1; dx++) {
			for (int dy = -1; dy <= 1; dy++) {
				if (ABS(dx) + ABS(dy) == 1) {
					int new_steps = next_step(maze, distmap, keys, num_keys, x, y, dx, dy, steps + 1);
					min_steps = MIN(min_steps, new_steps);
				}
			}
		}
	}
	return min_steps;
}

int next_step2(char maze[100][100], unsigned long distmap[100][100][10000], int doors[],
			  unsigned long keys, int num_keys, int xpos[4], int ypos[4],
			  int quadrant, int dx, int dy, unsigned long steps) {
	int x = xpos[quadrant] + dx;
	int y = ypos[quadrant] + dy;
	char chr = maze[y][x];
	if (chr == '#') {
		return INT_MAX;
	}
	if (chr >= 'A' && chr <= 'Z' && !((keys >> (chr - 'A')) & 1)) {
		return INT_MAX;
	}
	int key_found = 0;
	if (chr >= 'a' && chr <= 'z' && !((keys >> (chr - 'a')) & 1)) {
		keys |= 1 << (chr - 'a');
		num_keys--;
		if (num_keys == 0) {
			return steps;
		}
		key_found = 1;
	}
	if (get_dist_by_keymap(distmap, keys, x, y) <= steps) {
		return INT_MAX;
	}
	put_dist_by_keymap(distmap, keys, x, y, steps);

	xpos[quadrant] += dx;
	ypos[quadrant] += dy;
	int min_steps = INT_MAX;
	if ((dx || dy) && !key_found) {
		int new_steps = next_step2(
			maze, distmap, doors, keys, num_keys, xpos, ypos, quadrant, dx, dy, steps + 1
		);
		min_steps = MIN(min_steps, new_steps);
		if (dx) {
			new_steps = next_step2(
				maze, distmap, doors, keys, num_keys, xpos, ypos, quadrant, 0, -1, steps + 1
			);
			min_steps = MIN(min_steps, new_steps);
			new_steps = next_step2(
				maze, distmap, doors, keys, num_keys, xpos, ypos, quadrant, 0, 1, steps + 1
			);
			min_steps = MIN(min_steps, new_steps);
		} else {
			new_steps = next_step2(
				maze, distmap, doors, keys, num_keys, xpos, ypos, quadrant, -1, 0, steps + 1
			);
			min_steps = MIN(min_steps, new_steps);
			new_steps = next_step2(
				maze, distmap, doors, keys, num_keys, xpos, ypos, quadrant, 1, 0, steps + 1
			);
			min_steps = MIN(min_steps, new_steps);
		}
	} else {
		for (int quad = key_found ? 0 : quadrant; quad <= (key_found ? 3 : quadrant); quad++) {
			for (int dx = -1; dx <= 1; dx++) {
				for (int dy = -1; dy <= 1; dy++) {
					if (ABS(dx) + ABS(dy) == 1) {
						int new_steps = next_step2(
							maze, distmap, doors, keys, num_keys, xpos, ypos, quad, dx, dy, steps + 1
						);
						min_steps = MIN(min_steps, new_steps);
					}
				}
			}
		}
	}
	xpos[quadrant] -= dx;
	ypos[quadrant] -= dy;
	return min_steps;
}

int main() {
	time_t starttime = time(NULL);

	char maze[100][100];
	int max_y = 0;
	FILE* fp = fopen("input.txt", "r");
	while (fgets(maze[max_y], 100, fp) != NULL) {
		max_y++;
	}
	fclose(fp);

	// Search entrance "@" and all doors
	unsigned long keys = (1UL << (sizeof(unsigned long) * 8 - 1));
	int num_keys = 0;
	int xpos, ypos;
	for (int y = 0; y < max_y; y++) {
		for (int x = 0; x < strlen(maze[y]); x++) {
			char chr = maze[y][x];
			if (chr >= 'a' && chr <= 'z') {
				num_keys++;
			} else if (chr == '@') {
				xpos = x;
				ypos = y;
			}
		}
	}

	static unsigned long distmap[100][100][10000];
	int steps = next_step(maze, distmap, keys, num_keys, xpos, ypos, 0, 0, 0);
	printf("%d\n", steps);

	printf("Elapsed time part 1: %ld seconds\n", time(NULL) - starttime);
	starttime = time(NULL);

	// Part 2
	int doors[26];

	// Doors: 0 = top left quadrant, 1 = top right, 2 = bottom left, 3 = bottom right
	maze[ypos][xpos + 1] = '#';
	maze[ypos][xpos - 1] = '#';
	maze[ypos + 1][xpos] = '#';
	maze[ypos - 1][xpos] = '#';

	for (int y = 0; y < max_y; y++) {
		for (int x = 0; x < strlen(maze[y]); x++) {
			char chr = maze[y][x];
			if (chr >= 'A' && chr <= 'Z') {
				if (y < ypos) {
					if (x < xpos) {
						doors[chr - 'A'] = 0;
					} else {
						doors[chr - 'A'] = 1;
					}
				} else {
					if (x < xpos) {
						doors[chr - 'A'] = 2;
					} else {
						doors[chr - 'A'] = 3;
					}
				}
			}
		}
	}

	int x[4], y[4];
	x[0] = xpos - 1;
	x[1] = xpos + 1;
	x[2] = xpos - 1;
	x[3] = xpos + 1;
	y[0] = ypos - 1;
	y[1] = ypos - 1;
	y[2] = ypos + 1;
	y[3] = ypos + 1;

	static unsigned long distmap2[100][100][10000];
	int min_steps = INT_MAX;
	for (int quad = 0; quad < 4; quad++) {
		int steps = next_step2(maze, distmap2, doors, keys, num_keys, x, y, quad, 0, 0, 0);
		min_steps = MIN(min_steps, steps);
	}
	printf("%d\n", min_steps);

	printf("Elapsed time part 2: %ld seconds\n", time(NULL) - starttime);
}
