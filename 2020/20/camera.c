#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>


#define FILENAME "input.txt"


int tile_map[16][16];
int tile_rot_flip[256];
int tile_ids[256];
int tile_edges[256][4];
int tile_edges_rotflipped[256][8][4];
int tile_used[256];
int tile_content[256][8];
int num_tiles = 0;
int tiles_per_row;
int size = -1;


void print_pic(__uint128_t pic[]) {
	for (int y = 0; y < tiles_per_row * 8; y++) {
		for (int x = 0; x < tiles_per_row * 8; x++) {
			putchar(pic[y] & (__uint128_t) 1 << (tiles_per_row * 8 - x - 1) ? '#' : '.');
		}
		puts("");
	}
	puts("");
}

int find_pattern(const __uint128_t pic[], __uint128_t pic2[], const __uint128_t pattern[],
	int pat_x, int pat_y) {

	int num_monsters = 0;
	for (int y = 0; y < tiles_per_row * 8 - pat_y + 1; y++) {
		for (int x = 0; x < tiles_per_row * 8 - pat_x + 1; x++) {
			int match = 1;
			for (int pattern_y = 0; pattern_y < pat_y; pattern_y++) {
				if (~pic[y + pattern_y] & pattern[pattern_y] << x) {
					match = 0;
					break;
				}
			}
			if (match) {
				num_monsters++;
				for (int pattern_y = 0; pattern_y < pat_y; pattern_y++) {
					pic2[y + pattern_y] &= ~(pattern[pattern_y] << x);
				}
			}
		}
	}
	return num_monsters;
}

void search_monsters() {
	__uint128_t pic[128] = {0};
	for (int y_tile = 0; y_tile < tiles_per_row; y_tile++) {
		for (int x_tile = 0; x_tile < tiles_per_row; x_tile++) {
			int tile = tile_map[x_tile][y_tile];
			int rotflip = tile_rot_flip[tile];
			for (int y_to = 0; y_to < 8; y_to++) {
				pic[8 * y_tile + y_to] <<= 8;
				for (int x_to = 0; x_to < 8; x_to++) {
					int x_from, y_from;
					switch (rotflip) {
						case 0:
							x_from = x_to;
							y_from = y_to;
							break;
						case 1:
							x_from = 8 - y_to - 1;
							y_from = x_to;
							break;
						case 2:
							x_from = 8 - x_to - 1;
							y_from = 8 - y_to - 1;
							break;
						case 3:
							x_from = y_to;
							y_from = 8 - x_to - 1;
							break;
						case 4:
							x_from = 8 - x_to - 1;
							y_from = y_to;
							break;
						case 5:
							x_from = y_to;
							y_from = x_to;
							break;
						case 6:
							x_from = x_to;
							y_from = 8 - y_to - 1;
							break;
						case 7:
							x_from = 8 - y_to - 1;
							y_from = 8 - x_to - 1;
							break;
						default:
							assert(0);
					}
					int src_pix_set = tile_content[tile][y_from] & 1 << (8 - x_from - 1);
					pic[8 * y_tile + y_to] |= src_pix_set ? (__uint128_t) 1 << (8 - x_to - 1) : 0;
				}
			}
		}
	}
	// print_pic(pic);
	__uint128_t pic2[128];
	memcpy(pic2, pic, sizeof(pic));

	const __uint128_t pattern_horz[4][3] = {
		{
			0b00000000000000000010,
			0b10000110000110000111,
			0b01001001001001001000
		},
		{
			0b01000000000000000000,
			0b11100001100001100001,
			0b00010010010010010010
		},
		{
			0b01001001001001001000,
			0b10000110000110000111,
			0b00000000000000000010,
		},
		{
			0b00010010010010010010,
			0b11100001100001100001,
			0b01000000000000000000,
		}
	};
	const __uint128_t pattern_vert[4][20] = {
		{
			0b010,
			0b001,
			0b000,
			0b000,
			0b001,
			0b010,
			0b010,
			0b001,
			0b000,
			0b000,
			0b001,
			0b010,
			0b010,
			0b001,
			0b000,
			0b000,
			0b001,
			0b010,
			0b110,
			0b010,
		},
		{
			0b010,
			0b100,
			0b000,
			0b000,
			0b100,
			0b010,
			0b010,
			0b100,
			0b000,
			0b000,
			0b100,
			0b010,
			0b010,
			0b100,
			0b000,
			0b000,
			0b100,
			0b010,
			0b011,
			0b010,
		},
		{
			0b010,
			0b110,
			0b010,
			0b001,
			0b000,
			0b000,
			0b001,
			0b010,
			0b010,
			0b001,
			0b000,
			0b000,
			0b001,
			0b010,
			0b010,
			0b001,
			0b000,
			0b000,
			0b001,
			0b010,
		},
		{
			0b010,
			0b011,
			0b010,
			0b100,
			0b000,
			0b000,
			0b100,
			0b010,
			0b010,
			0b100,
			0b000,
			0b000,
			0b100,
			0b010,
			0b010,
			0b100,
			0b000,
			0b000,
			0b100,
			0b010,
		},
	};
	int num_monsters = 0;
	for (int pattern = 0; pattern < 4; pattern++) {
		num_monsters += find_pattern(pic, pic2, pattern_horz[pattern], 20, 3);
		num_monsters += find_pattern(pic, pic2, pattern_vert[pattern], 3, 20);
	}
	printf("  ---> Monsters found: %d\n", num_monsters);
	int roughness = 0;
	for (int y = 0; y < tiles_per_row * 8; y++) {
		while (pic2[y]) {
			roughness += (int) (pic2[y] & 1);
			pic2[y] >>= 1;
		}
	}
	printf("  ---> Roughness:      %d\n", roughness);
}

void next(int next_step) {
	if (next_step >= num_tiles) {
		printf("Solution found: %ld\n",
			(long) tile_ids[tile_map[0][0]] *
			tile_ids[tile_map[tiles_per_row - 1][0]] *
			tile_ids[tile_map[0][tiles_per_row - 1]] *
			tile_ids[tile_map[tiles_per_row - 1][tiles_per_row - 1]]);
		search_monsters();
	}

	int x = next_step % tiles_per_row;
	int y = next_step / tiles_per_row;

	for (int tile = 0; tile < num_tiles; tile++) {
		tile_map[x][y] = tile;
		if (!tile_used[tile]) {
			tile_used[tile] = 1;
			for (int rotflip = 0; rotflip < 8; rotflip++) {
				if (x > 0) {
					int tile_left = tile_map[x - 1][y];
					int tile_left_edge = tile_edges_rotflipped[tile_left][tile_rot_flip[tile_left]][2];
					int tile_side = tile_edges_rotflipped[tile][rotflip][1];
					if (tile_side != tile_left_edge) {
						continue;
					}
				}
				if (y > 0) {
					int tile_above = tile_map[x][y - 1];
					int tile_above_side = tile_edges_rotflipped[tile_above][tile_rot_flip[tile_above]][3];
					int tile_side = tile_edges_rotflipped[tile][rotflip][0];
					if (tile_side != tile_above_side) {
						continue;
					}
				}
				tile_rot_flip[tile] = rotflip;
				next(next_step + 1);
			}
			tile_used[tile] = 0;
		}
	}
}

int reverse(int data, int size) {
	int result = 0;
	for (int bit = 0; bit < size; bit++) {
		result |= data & 1;
		result <<= 1;
		data >>= 1;
	}
	return result >> 1;
}

int line2content(char line[]) {
	int content = 0;
	for (int i = 1; i < 9; i++) {
		content = content << 1 | (line[i] == '#' ? 1 : 0);
	}
	return content;
}

int main() {
	FILE* fp = fopen(FILENAME, "r");
	while (1) {
		char line[50];
		fgets(line, 50, fp);
		if (feof(fp)) {
			break;
		}
		assert(!strncmp(line, "Tile ", 5));
		int tile_id = atoi(line + 5);
		fgets(line, 50, fp);
		assert(size < 0 || size == (int) strlen(line) - 1);
		size = (int) strlen(line) - 1;
		assert(size == 10);
		int side_top = 0, side_left = 0, side_right = 0, side_bottom = 0;
		for (int bit = 0; bit < size; bit++) {
			side_top = side_top << 1 | (line[bit] == '#' ? 1 : 0);
		}
		side_left = line[0] == '#' ? 1 : 0;
		side_right = line[size - 1] == '#' ? 1 : 0;
		for (int i = 1; i < size; i++) {
			fgets(line, 50, fp);
			assert((int) strlen(line) - 1 == size);
			side_left = side_left << 1 | (line[0] == '#' ? 1 : 0);
			side_right = side_right << 1 | (line[size - 1] == '#' ? 1 : 0);
			tile_content[num_tiles][i - 1] = line2content(line);
		}
		for (int bit = 0; bit < size; bit++) {
			side_bottom = side_bottom << 1 | (line[bit] == '#' ? 1 : 0);
		}
		fgets(line, 50, fp);
		assert(line[0] == '\n' || feof(fp));
		tile_ids[num_tiles] = tile_id;
		tile_edges[num_tiles][0] = side_top;
		tile_edges[num_tiles][1] = side_left;
		tile_edges[num_tiles][2] = side_right;
		tile_edges[num_tiles][3] = side_bottom;
		num_tiles++;
	}
	fclose(fp);

	while (tiles_per_row * tiles_per_row < num_tiles) {
		tiles_per_row++;
	}
	assert(tiles_per_row * tiles_per_row == num_tiles);

	for (int tile = 0; tile < num_tiles; tile++) {
		tile_edges_rotflipped[tile][0][0] = tile_edges[tile][0];
		tile_edges_rotflipped[tile][0][1] = tile_edges[tile][1];
		tile_edges_rotflipped[tile][0][2] = tile_edges[tile][2];
		tile_edges_rotflipped[tile][0][3] = tile_edges[tile][3];

		tile_edges_rotflipped[tile][1][0] = tile_edges[tile][2];
		tile_edges_rotflipped[tile][1][1] = reverse(tile_edges[tile][0], size);
		tile_edges_rotflipped[tile][1][2] = reverse(tile_edges[tile][3], size);
		tile_edges_rotflipped[tile][1][3] = tile_edges[tile][1];

		tile_edges_rotflipped[tile][2][0] = reverse(tile_edges[tile][3], size);
		tile_edges_rotflipped[tile][2][1] = reverse(tile_edges[tile][2], size);
		tile_edges_rotflipped[tile][2][2] = reverse(tile_edges[tile][1], size);
		tile_edges_rotflipped[tile][2][3] = reverse(tile_edges[tile][0], size);

		tile_edges_rotflipped[tile][3][0] = reverse(tile_edges[tile][1], size);
		tile_edges_rotflipped[tile][3][1] = tile_edges[tile][3];
		tile_edges_rotflipped[tile][3][2] = tile_edges[tile][0];
		tile_edges_rotflipped[tile][3][3] = reverse(tile_edges[tile][2], size);

		tile_edges_rotflipped[tile][4][0] = reverse(tile_edges[tile][0], size);
		tile_edges_rotflipped[tile][4][1] = tile_edges[tile][2];
		tile_edges_rotflipped[tile][4][2] = tile_edges[tile][1];
		tile_edges_rotflipped[tile][4][3] = reverse(tile_edges[tile][3], size);

		tile_edges_rotflipped[tile][5][0] = tile_edges_rotflipped[tile][4][2];
		tile_edges_rotflipped[tile][5][1] = reverse(tile_edges_rotflipped[tile][4][0], size);
		tile_edges_rotflipped[tile][5][2] = reverse(tile_edges_rotflipped[tile][4][3], size);
		tile_edges_rotflipped[tile][5][3] = tile_edges_rotflipped[tile][4][1];

		tile_edges_rotflipped[tile][6][0] = reverse(tile_edges_rotflipped[tile][4][3], size);
		tile_edges_rotflipped[tile][6][1] = reverse(tile_edges_rotflipped[tile][4][2], size);
		tile_edges_rotflipped[tile][6][2] = reverse(tile_edges_rotflipped[tile][4][1], size);
		tile_edges_rotflipped[tile][6][3] = reverse(tile_edges_rotflipped[tile][4][0], size);

		tile_edges_rotflipped[tile][7][0] = reverse(tile_edges_rotflipped[tile][4][1], size);
		tile_edges_rotflipped[tile][7][1] = tile_edges_rotflipped[tile][4][3];
		tile_edges_rotflipped[tile][7][2] = tile_edges_rotflipped[tile][4][0];
		tile_edges_rotflipped[tile][7][3] = reverse(tile_edges_rotflipped[tile][4][2], size);
	}

	next(0);
}
