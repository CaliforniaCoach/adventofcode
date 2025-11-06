#include <stdio.h>
#include <assert.h>
#include <string.h>


#define FILENAME "input.txt"

#define MAX_SIZE 100
#define LINES 8

#define OFFSET ((MAX_SIZE - LINES) / 2)

char playfield1[MAX_SIZE][MAX_SIZE][MAX_SIZE];
char playfield2[MAX_SIZE][MAX_SIZE][MAX_SIZE];

char playfield4_1[MAX_SIZE][MAX_SIZE][MAX_SIZE][MAX_SIZE];
char playfield4_2[MAX_SIZE][MAX_SIZE][MAX_SIZE][MAX_SIZE];

void print(char playfield[][MAX_SIZE][MAX_SIZE]) {
	for (int y = 45; y < 50; y++) {
		for (int x = 45; x < 50; x++) {
			if (playfield[x][y][OFFSET]) {
				printf("#");
			} else {
				printf(".");
			}
		}
		printf("\n");
	}
}

void cycle(char pf1[][MAX_SIZE][MAX_SIZE], char pf2[][MAX_SIZE][MAX_SIZE]) {
	memset(pf2, 0, MAX_SIZE * MAX_SIZE * MAX_SIZE);
	for (int x = 1; x < MAX_SIZE - 1; x++) {
		for (int y = 1; y < MAX_SIZE - 1; y++) {
			for (int z = 1; z < MAX_SIZE - 1; z++) {
				if (pf1[x][y][z]) {
					for (int dx = -1; dx <= 1; dx++) {
						for (int dy = -1; dy <= 1; dy++) {
							for (int dz = -1; dz <= 1; dz++) {
								if (dx || dy || dz) {
									pf2[x + dx][y + dy][z + dz]++;
								}
							}
						}
					}
				}
			}
		}
	}
	for (int x = 1; x < MAX_SIZE - 1; x++) {
		for (int y = 1; y < MAX_SIZE - 1; y++) {
			for (int z = 1; z < MAX_SIZE - 1; z++) {
				if (pf2[x][y][z] == 3 || (pf1[x][y][z] && pf2[x][y][z] == 2)) {
					pf2[x][y][z] = 1;
				} else {
					pf2[x][y][z] = 0;
				}
			}
		}
	}
}

unsigned long count(char playfield[][MAX_SIZE][MAX_SIZE]) {
	unsigned long count = 0;
	for (int x = 0; x < MAX_SIZE; x++) {
		for (int y = 0; y < MAX_SIZE; y++) {
			for (int z = 0; z < MAX_SIZE; z++) {
				if (playfield[x][y][z]) {
					count++;
				}
			}
		}
	}
	return count;
}

void cycle4(char pf1[][MAX_SIZE][MAX_SIZE][MAX_SIZE], char pf2[][MAX_SIZE][MAX_SIZE][MAX_SIZE]) {
	memset(pf2, 0, MAX_SIZE * MAX_SIZE * MAX_SIZE * MAX_SIZE);
	for (int x = 1; x < MAX_SIZE - 1; x++) {
		for (int y = 1; y < MAX_SIZE - 1; y++) {
			for (int z = 1; z < MAX_SIZE - 1; z++) {
				for (int w = 1; w < MAX_SIZE - 1; w++) {
					if (pf1[x][y][z][w]) {
						for (int dx = -1; dx <= 1; dx++) {
							for (int dy = -1; dy <= 1; dy++) {
								for (int dz = -1; dz <= 1; dz++) {
									for (int dw = -1; dw <= 1; dw++) {
										if (dx || dy || dz || dw) {
											pf2[x + dx][y + dy][z + dz][w + dw]++;
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	for (int x = 1; x < MAX_SIZE - 1; x++) {
		for (int y = 1; y < MAX_SIZE - 1; y++) {
			for (int z = 1; z < MAX_SIZE - 1; z++) {
				for (int w = 1; w < MAX_SIZE - 1; w++) {
					if (pf2[x][y][z][w] == 3 || (pf1[x][y][z][w] && pf2[x][y][z][w] == 2)) {
						pf2[x][y][z][w] = 1;
					} else {
						pf2[x][y][z][w] = 0;
					}
				}
			}
		}
	}
}

unsigned long count4(char playfield[][MAX_SIZE][MAX_SIZE][MAX_SIZE]) {
	unsigned long count = 0;
	for (int x = 0; x < MAX_SIZE; x++) {
		for (int y = 0; y < MAX_SIZE; y++) {
			for (int z = 0; z < MAX_SIZE; z++) {
				for (int w = 0; w < MAX_SIZE; w++) {
					if (playfield[x][y][z][w]) {
						count++;
					}
				}
			}
		}
	}
	return count;
}

int main() {
	int x = 0, y = 0;
	FILE* fp = fopen(FILENAME, "r");
	while (!feof(fp)) {
		int chr = fgetc(fp);
		switch (chr) {
			case '.':
				x++;
				break;
			case '#':
				playfield1[OFFSET + x][OFFSET + y][OFFSET] = 1;
				playfield4_1[OFFSET + x++][OFFSET + y][OFFSET][OFFSET] = 1;
				break;
			case '\n':
				x = 0;
				y++;
				break;
			default:
				assert(1);
		}
	}
	fclose(fp);

	char (*pf1)[MAX_SIZE][MAX_SIZE] = playfield1;
	char (*pf2)[MAX_SIZE][MAX_SIZE] = playfield2;

	for (int i = 0; i < 6; i++) {
		printf("after %d: count = %ld\n", i, count(pf1));
		cycle(pf1, pf2);
		char (*temp)[MAX_SIZE][MAX_SIZE] = pf1;
		pf1 = pf2;
		pf2 = temp;
	}
	printf("Final count: %ld\n\n", count(pf1));


	char (*pf4_1)[MAX_SIZE][MAX_SIZE][MAX_SIZE] = playfield4_1;
	char (*pf4_2)[MAX_SIZE][MAX_SIZE][MAX_SIZE] = playfield4_2;

	for (int i = 0; i < 6; i++) {
		printf("after %d: count = %ld\n", i, count4(pf4_1));
		cycle4(pf4_1, pf4_2);
		char (*temp)[MAX_SIZE][MAX_SIZE][MAX_SIZE] = pf4_1;
		pf4_1 = pf4_2;
		pf4_2 = temp;
	}
	printf("Final count: %ld\n", count4(pf4_1));
}
