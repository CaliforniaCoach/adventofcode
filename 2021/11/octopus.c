#include <stdio.h>
#include <assert.h>

int flash(int field[10][10], int x, int y) {
	int count = 0;
	for (int dx = -1; dx <= 1; dx++) {
		for (int dy = -1; dy <= 1; dy++) {
			int tx = x + dx;
			int ty = y + dy;
			if ((dx || dy) && tx >= 0 && tx < 10 && ty >= 0 && ty < 10) {
				if (++field[y + dy][x + dx] == 10) {
					count += 1 + flash(field, x + dx, y + dy);
				}
			}
		}
	}
	return count;
}

int run_step(int field[10][10]) {
	int count = 0;
	for (int y = 0; y < 10; y++) {
		for (int x = 0; x < 10; x++) {
			if (++field[y][x] == 10) {
				count += 1 + flash(field, x, y);
			}
		}
	}
	int count2 = 0;
	for (int y = 0; y < 10; y++) {
		for (int x = 0; x < 10; x++) {
			if (field[y][x] >= 10) {
				field[y][x] = 0;
				count2++;
			}
		}
	}
	assert(count == count2);
	return count;
}

int main() {
	int field[10][10];
	FILE* fp = fopen("input.txt", "r");
	for (int y = 0; y < 10; y++) {
		for (int x = 0; x < 10; x++) {
			field[y][x] = getc(fp) - '0';
		}
		assert(getc(fp) == '\n');
	}
	fclose(fp);

	int count = 0;
	int all_flashed = 0;
	for (int step = 0; step < 100 || !all_flashed; step++) {
		int num_flashed = run_step(field);
		if (step < 100) {
			count += num_flashed;
		}
		if (!all_flashed && num_flashed == 10 * 10) {
			all_flashed = step;
		}
	}
	printf("%d\n%d\n", count, all_flashed + 1);
}
