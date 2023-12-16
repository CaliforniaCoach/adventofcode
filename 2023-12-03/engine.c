#include <stdio.h>
#include <string.h>


#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))


int is_digit(char chr) {
	return chr >= '0' && chr <= '9';
}


int main(int argc, char** argv) {

	char schematic[1000][1000] = {'\0'};

	FILE *f = fopen("input.txt", "r");
	int line = 0;
	while (fgets(schematic[line++], 1000, f));
	fclose(f);
	line--;

	long sum = 0;
	for (int y = 0; y < line; y++) {
		// Trailing \n line break shall not be considered by line_len
		int line_len = strlen(schematic[y]) - 1;
		for (int x = 0; x < line_len; x++) {
			if (schematic[y][x] >= '0' && schematic[y][x] <= '9') {
				int num = atoi(schematic[y] + x);
				int num_end = x;
				// Advance num_end to next non-digit field
				while (schematic[y][++num_end] >= '0' && schematic[y][num_end] <= '9');
				int is_part_number = 0;
				for (int x_search = MAX(x - 1, 0); x_search <= MIN(num_end, line_len - 1); x_search++) {
					if (y > 0) {
						if (schematic[y - 1][x_search] != '.') {
							is_part_number = 1;
						}
					}
					if (y < line - 1) {
						if (schematic[y + 1][x_search] != '.') {
							is_part_number = 1;
						}
					}
				}
				if (x > 0 && schematic[y][x - 1] != '.') {
					is_part_number = 1;
				}
				if (num_end < line_len && schematic[y][num_end] != '.') {
					is_part_number = 1;
				}
				if (is_part_number) {
					printf("%d ", num);
					sum += num;
				}
				x = num_end;
			}
		}
	}

	printf("\n%d\n", sum);


	/////////////////////// Part 2 /////////////////////////////

	sum = 0;
	for (int y = 0; y < line; y++) {
		// Trailing \n line break shall not be considered by line_len
		int line_len = strlen(schematic[y]) - 1;
		for (int x = 0; x < line_len; x++) {
			if (schematic[y][x] == '*') {
				int num_numbers = 0, is_gear = 0, gear_ratio = 1;

				if (y > 0) {
					for (int x_search = MAX(x - 1, 0); x_search <= MIN(x + 1, line_len - 1); x_search++) {
						if (is_digit(schematic[y - 1][x_search])) {
							int num_start = x_search, num_end = x_search;
							while (--num_start >= 0 && is_digit(schematic[y - 1][num_start]));
							num_start++;
							int num = atoi(schematic[y - 1] + num_start);
							printf("%d ", num);
							gear_ratio *= num;
							num_numbers++;
							while (++num_end < line_len && is_digit(schematic[y - 1][num_end]));
							x_search = num_end;
						}
					}
				}
				if (y < line - 1) {
					for (int x_search = MAX(x - 1, 0); x_search <= MIN(x + 1, line_len - 1); x_search++) {
						if (is_digit(schematic[y + 1][x_search])) {
							int num_start = x_search, num_end = x_search;
							while (--num_start >= 0 && is_digit(schematic[y + 1][num_start]));
							num_start++;
							int num = atoi(schematic[y + 1] + num_start);
							printf("%d ", num);
							gear_ratio *= num;
							num_numbers++;
							while (++num_end < line_len && is_digit(schematic[y + 1][num_end]));
							x_search = num_end;
						}
					}
				}
				if (x > 0) {
					if (is_digit(schematic[y][x - 1])) {
						int num_start = x - 1;
						while (--num_start >= 0 && is_digit(schematic[y][num_start]));
						num_start++;
						int num = atoi(schematic[y] + num_start);
						printf("%d ", num);
						gear_ratio *= num;
						num_numbers++;
					}
				}
				if (x < line_len - 1) {
					if (is_digit(schematic[y][x + 1])) {
						int num = atoi(schematic[y] + x + 1);
						printf("%d ", num);
						gear_ratio *= num;
						num_numbers++;
					}
				}
				if (num_numbers == 2) {
					sum += gear_ratio;
					printf("-> %d -> %d\n", gear_ratio, sum);
				}
			}
		}
	}

	printf("\n%d\n", sum);
}
