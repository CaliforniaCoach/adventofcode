#include <stdio.h>
#include <string.h>
#include <assert.h>

#define ABS(x) ((x) >= 0 ? (x) : -(x))

long bugs2rating(int bugs[][5], int rows, int columns) {
	long rating = 0L;
	for (int y = rows - 1; y >= 0; y--) {
		for (int x = columns - 1; x >= 0; x--) {
			rating = (rating << 1) | (bugs[y][x] ? 1 : 0);
		}
	}
	return rating;
}

void print_bugs(int bugs[][5], int rows, int columns) {
	for (int y = 0; y < rows; y++) {
		for (int x = 0; x < columns; x++) {
			putc(bugs[y][x] ? '#' : '.', stdout);
		}
		puts("");
	}
}

void print_multibugs(int multibugs[][5][5]) {
	for (int level = 0; level < 201; level++) {
		int (*bugs)[5] = multibugs[level];
		if (bugs2rating(bugs, 5, 5)) {
			printf("Level %d = depth %d:\n", level - 100, -(level - 100));
			print_bugs(bugs, 5, 5);
			puts("");
		}
	}
}

void step(int bugs_current[][5], int bugs_new[][5], int rows, int columns) {
	for (int y = 0; y < rows; y++) {
		for (int x = 0; x < columns; x++) {
			int adjacent_bugs = 0;
			for (int dx = -1; dx <= 1; dx++) {
				for (int dy = -1; dy <= 1; dy++) {
					if (ABS(dx) + ABS(dy) == 1) {
						int x2 = x + dx;
						int y2 = y + dy;
						if (x2 >= 0 && x2 < columns && y2 >= 0 && y2 < rows) {
							if (bugs_current[y2][x2]) {
								adjacent_bugs++;
							}
						}
					}
				}
			}
			bugs_new[y][x] = (adjacent_bugs == 1) || (!bugs_current[y][x] && (adjacent_bugs == 2));
		}
	}
}

void step2(int bugs_current[][5][5], int bugs_new[][5][5], int rows, int cols, int levels) {
	for (int level = 0; level < levels; level++) {
		for (int y = 0; y < rows; y++) {
			for (int x = 0; x < cols; x++) {
				int adjacent_bugs = 0;
				for (int dx = -1; dx <= 1; dx++) {
					for (int dy = -1; dy <= 1; dy++) {
						if (ABS(dx) + ABS(dy) == 1 && !(x == 2 && y == 2)) {
							int x2 = x + dx;
							int y2 = y + dy;
							if (x2 < 0 || x2 >= cols) {
								if (level + 1 < levels) {
									if (bugs_current[level + 1][2][x2 < 0 ? 1 : 3]) {
										adjacent_bugs++;
									}
								} else {
									assert(!bugs_current[level][y][x]);
								}
							}
							else if (y2 < 0 || y2 >= rows) {
								if (level + 1 < levels) {
									if (bugs_current[level + 1][y2 < 0 ? 1 : 3][2]) {
										adjacent_bugs++;
									}
								} else {
									assert(!bugs_current[level][y][x]);
								}
							}
							else if (x2 == 2 && y2 == 2) {
								if (level - 1 >= 0) {
									if (y == 2) {
										for (int y3 = 0; y3 < rows; y3++) {
											if (bugs_current[level - 1][y3][x < 2 ? 0 : 4]) {
												adjacent_bugs++;
											}
										}
									} else {
										for (int x3 = 0; x3 < cols; x3++) {
											if (bugs_current[level - 1][y < 2 ? 0 : 4][x3]) {
												adjacent_bugs++;
											}
										}
									}
								} else {
									assert(!bugs_current[level][y][x]);
								}
							}
							else {
								if (bugs_current[level][y2][x2]) {
									adjacent_bugs++;
								}
							}
						}
					}
				}
				bugs_new[level][y][x] = (adjacent_bugs == 1) || 
				                        (!bugs_current[level][y][x] && (adjacent_bugs == 2));
			}
		}
	}
}

int main() {
	int bugs[5][5];
	char buf[100];
	FILE* fp = fopen("input.txt", "r");
	int row = 0;
	while (fgets(buf, 100, fp) != NULL) {
		int i = 0;
		while (buf[i] != '\n' && buf[i] != 0) {
			bugs[row][i] = buf[i] == '#' ? 1 : 0;
			i++;
		}
		row++;
	}
	fclose(fp);

	int bugs1[5][5];
	memcpy(bugs1, bugs, 5 * 5 * sizeof(int));
	int bugs2[5][5];
	int (*pBugs1)[5] = bugs1;
	int (*pBugs2)[5] = bugs2;
	long ratings[100000];
	int num_ratings = 0;
	int exit = 0;

	while (1) {
		step(pBugs1, pBugs2, 5, 5);
		long rating = bugs2rating(pBugs2, 5, 5);
		for (int i = 0; i < num_ratings; i++) {
			if (rating == ratings[i]) {
				print_bugs(pBugs2, 5, 5);
				printf("Part 1 solution: %ld\n", rating);
				exit = 1;
				break;
			}
		}
		if (exit) {
			break;
		}
		ratings[num_ratings++] = rating;
		int (*swap)[5] = pBugs2;
		pBugs2 = pBugs1;
		pBugs1 = swap;
	}

	// Part 2

	// In 200 minutes, max. levels from -100 to +100 can become populated -> level 0 is index 100
	int multibugs1[201][5][5] = {0};
	memcpy(multibugs1[100], bugs, 5 * 5 * sizeof(int));

	int multibugs2[201][5][5] = {0};
	int (*pmb1)[5][5] = multibugs1;
	int (*pmb2)[5][5] = multibugs2;
	for (int minute = 0; minute < 200; minute++) {
		step2(pmb1, pmb2, 5, 5, 201);
		int (*swap)[5][5] = pmb2;
		pmb2 = pmb1;
		pmb1 = swap;
	}

	int count = 0;
	for (int level = 0; level < 201; level++) {
		for (int y = 0; y < 5; y++) {
			for (int x = 0; x < 5; x++) {
				count += pmb1[level][y][x] ? 1 : 0;
			}
		}
	}
	printf("Part 2 solution: %d\n", count);
}
