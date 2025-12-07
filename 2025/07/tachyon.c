#include <stdio.h>
#include <assert.h>
#include <limits.h>

#define MAX_ROWS 500
#define MAX_COLS 500

int main() {
	int splitters[MAX_ROWS][MAX_COLS] = {0};
	int start_position;
	int num_rows = 0;
	int col = 0;
	FILE* fp = fopen("input.txt", "r");
	while (1) {
		int chr = getc(fp);
		if (feof(fp)) {
			break;
		}
		switch (chr) {
			case 'S':
				assert(num_rows == 0);
				start_position = col;
				break;
			case '^':
				assert(num_rows > 0);
				splitters[num_rows][col] = 1;
				break;
			case '.':
				break;
			case '\n':
				col = -1;
				num_rows++;
				break;
			default:
				assert(0);
		}
		col++;
	}
	fclose(fp);

	long beams[MAX_COLS] = {0};
	beams[start_position] = 1;
	int num_splits = 0;
	for (int row = 1; row < num_rows; row++) {
		for (int col = 0; col < 1000; col++) {
			if (beams[col] && splitters[row][col]) {
				assert(!splitters[row][col - 1] && !splitters[row][col + 1]);
				assert(LONG_MAX - beams[col] >= beams[col - 1]);
				assert(LONG_MAX - beams[col] >= beams[col + 1]);
				beams[col - 1] += beams[col];
				beams[col + 1] += beams[col];
				beams[col] = 0;
				num_splits++;
			}
		}
	}
	long timelines = 0;
	for (int i = 0; i < 1000; i++) {
		timelines += beams[i];
	}
	printf("%d\n%ld\n", num_splits, timelines);
}
