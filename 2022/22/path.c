#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


#define MAX_LINE_BUFFER 10000

const int X_DIR[] = {1, 0, -1, 0};
const int Y_DIR[] = {0, 1, 0, -1};
const char DIR_SYMBOL[] = {'>', 'v', '<', '^'};


void print_field(char* field, int lines, int width) {
	for (int k = 0; k < width; k++)
		printf("-");
	printf("\n");
	for (int i = 0; i < lines; i++) {
		for (int k = 0; k < width; k++) {
			printf("%c", field[i * width + k]);
		}
		puts("|");
	}
	for (int k = 0; k < width; k++)
		printf("-");
	printf("\n");
}


void walk_path_part2(char* input_field, int width, int height, int x, int y, int dir, char path[]) {
	char field[height][width];
	memcpy(field, input_field, width * height * sizeof(char));
	field[y][x] = DIR_SYMBOL[dir];
	char* path_ptr = path;
	while (*path_ptr != '\0') {
		switch (*path_ptr) {
			case 'R':
				dir = (dir + 1 + 4) % 4;
				field[y][x] = DIR_SYMBOL[dir];
				path_ptr++;
				break;
			case 'L':
				dir = (dir - 1 + 4) % 4;
				field[y][x] = DIR_SYMBOL[dir];
				path_ptr++;
				break;
			default:
				int steps = 0;
				do {
					steps *= 10;
					steps += *(path_ptr++) - '0';
				} while (*path_ptr >= '0' && *path_ptr <= '9');
				for (int step = 0; step < steps; step++) {
					int next_x = x + X_DIR[dir];
					int next_y = y + Y_DIR[dir];
					int next_dir = dir;
					if (next_y < 0) {
						if (next_x >= 50 && next_x < 100) {
							next_y = next_x - 50 + 150;
							next_x = 0;
							assert(dir == 3);
							next_dir = 0;
						}
						else if (next_x >= 100) {
							assert(dir == 3);
							next_y = 199;
							next_x = next_x - 100;
						}
					}
					else if (next_y < 50) {
						if (next_x < 50) {
							assert(dir == 2);
							next_y = 149 - next_y;
							next_x = 0;
							next_dir = 0;
						}
						else if (next_x >= 150) {
							assert(dir == 0);
							next_y = 149 - next_y;
							next_x = 99;
							next_dir = 2;
						}
					}
					else if (next_y < 100) {
						if (next_x < 50) {
							if (dir == 2) {
								next_y = 100;
								next_x = next_y - 50;
								next_dir = 1;
							}
							else {
								assert(dir == 3);
								next_y = next_x + 50;
								next_x = 50;
								next_dir = 0;
							}
						}
						else if (next_x >= 100) {
							if (dir == 0) {
								next_y = 49;
								next_x = 100 + next_y - 50;
								next_dir = 3;
							}
							else {
								assert(dir == 1);
								next_y = next_x - 100 + 50;
								next_x = 99;
								next_dir = 2;
							}
						}
					}
					else if (next_y < 150) {
						if (next_x < 0) {
							assert(dir == 2);
							next_y = 149 - next_y;
							next_x = 50;
							next_dir = 0;
						}
						else if (next_x >= 100) {
							assert(dir == 0);
							next_y = 149 - next_y;
							next_x = 149;
							next_dir = 2;
						}
					}
					else if (next_y < 200) {
						if (next_x < 0) {
							assert(dir == 2);
							next_x = 50 + next_y - 150;
							next_y = 0;
							next_dir = 1;
						}
						else if (next_x >= 50) {
							if (dir == 0) {
								next_y = 149;
								next_x = 50 + next_y - 150;
								next_dir = 3;
							}
							else {
								assert(dir == 1);
								next_y = 150 + next_x - 50;
								next_x = 49;
								next_dir = 2;
							}
						}
					}
					else {
						assert(next_y == 200);
						assert(dir == 1);
						next_y = 0;
						next_x = next_x + 100;
					}
					assert(next_x >= 0 && next_x < width && next_y >= 0 && next_y < height);
					assert(field[next_y][next_x] != ' ');
					if (field[next_y][next_x] == '#') {
						break;
					}
					else {
						x = next_x;
						y = next_y;
						dir = next_dir;
						field[y][x] = DIR_SYMBOL[dir];
					}
				}
		}
		// print_field((char*)field, height, width);
	}
	printf("\n*************************************************************\n");
	print_field((char*)field, height, width);
	printf("x, y, dir = %d, %d, %d ==> password = %d\n", x, y, dir, 1000 * (y + 1) + 4 * (x + 1) + dir);
}


int main(int argc, char* argv[]) {

	char linebuf[MAX_LINE_BUFFER];

	int max_line_length = 0;
	int num_lines_field = 0;

	// Count number of lines and maximum line length first
	FILE* f = fopen(argv[1], "r");
	while (fgets(linebuf, MAX_LINE_BUFFER, f)) {
		if (linebuf[0] == ' ' || linebuf[0] == '.' || linebuf[0] == '#') {
			num_lines_field++;
			int length = strlen(linebuf);
			if (length > max_line_length) {
				max_line_length = length;
			}
		}
	}
	fclose(f);

	// Field width is one less than max string length, because strings include closing '\n' char
	int width = max_line_length - 1;
	int height = num_lines_field;

	// Allocate field, then read it in from the file again
	char field[height][width];
	char path[MAX_LINE_BUFFER];

	f = fopen(argv[1], "r");
	int line = 0;
	while (fgets(linebuf, MAX_LINE_BUFFER, f)) {
		if (linebuf[0] == ' ' || linebuf[0] == '.' || linebuf[0] == '#') {
			int len = strlen(linebuf);
			strcpy(field[line], linebuf);
			memset(field[line] + len - 1, ' ', width - len + 1);
			line++;
		}
		else if (strlen(linebuf) > 0) {
			strcpy(path, linebuf);
		}
	}
	fclose(f);

	print_field((char*)field, height, width);

	char field_copy[height][width];
	memcpy(field_copy, field, width * height * sizeof(char));

	// Walk the path
	int x = 0;
	int y = 0;
	int dir = 0;
	char* path_ptr = path;
	while (*path_ptr != '\0') {
		switch (*path_ptr) {
			case 'R':
				dir = (dir + 1 + 4) % 4;
				field[y][x] = DIR_SYMBOL[dir];
				path_ptr++;
				break;
			case 'L':
				dir = (dir - 1 + 4) % 4;
				field[y][x] = DIR_SYMBOL[dir];
				path_ptr++;
				break;
			default:
				int steps = 0;
				do {
					steps *= 10;
					steps += *(path_ptr++) - '0';
				} while (*path_ptr >= '0' && *path_ptr <= '9');
				for (int step = 0; step < steps; step++) {
					int next_x = x;
					int next_y = y;
					do {
						next_x = (next_x + X_DIR[dir] + width) % width;
						next_y = (next_y + Y_DIR[dir] + height) % height;
					} while (field[next_y][next_x] == ' ');
					if (field[next_y][next_x] == '#') {
						break;
					}
					else {
						x = next_x;
						y = next_y;
						field[y][x] = DIR_SYMBOL[dir];
					}
				}
		}
	}
	printf("\n*************************************************************\n");
	print_field((char*)field, height, width);
	printf("x, y, dir = %d, %d, %d ==> password = %d\n", x, y, dir, 1000 * (y + 1) + 4 * (x + 1) + dir);

	memcpy(field, field_copy, width * height * sizeof(char));

	// Some tests
	walk_path_part2((char*)field, width, height, 51, 0, 3, "25");
	walk_path_part2((char*)field, width, height, 99, 0, 3, "25");
	walk_path_part2((char*)field, width, height, 100, 0, 3, "25");
	walk_path_part2((char*)field, width, height, 149, 0, 3, "25");

	walk_path_part2((char*)field, width, height, 50, 1, 2, "25");
	walk_path_part2((char*)field, width, height, 50, 49, 2, "25");

	walk_path_part2((char*)field, width, height, 148, 1, 0, "25");
	walk_path_part2((char*)field, width, height, 148, 49, 0, "25");

	walk_path_part2((char*)field, width, height, 100, 48, 1, "25");
	walk_path_part2((char*)field, width, height, 149, 48, 1, "25");

	// Walk the path again, with cube rules
	walk_path_part2((char*)field, width, height, 50, 0, 0, path);
}
