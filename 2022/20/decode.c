#include <stdio.h>
#include <stdlib.h>


#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))

#define SIGN(x) ((x) < 0 ? -1 : ((x) > 0 ? 1 : 0))

long mod(long x, long y) {
	if (x >= 0) {
		return x % y;
	}
	else {
		return (y - (-x % y)) % y;
	}
}


int main(int argc, char* argv[]) {

	char line[99];

	int factor = 1;
	int times = 1;
	if (argc >= 4) {
		factor = atoi(argv[2]);
		times = atoi(argv[3]);
	}

	// Count the number of lines = number of numbers
	int num_lines = 0;
	FILE* f = fopen(argv[1], "r");
	while (fgets(line, 99, f)) {
		num_lines++;
	}
	fclose(f);

	int original_order[num_lines];
	int backlink[num_lines];
	signed long dynamic_buffer[num_lines];
	f = fopen(argv[1], "r");
	int i = 0;
	while (fgets(line, 99, f)) {
		signed long number = atol(line);
		original_order[i] = i;
		backlink[i] = i;
		dynamic_buffer[i] = number * factor;
		i++;
	}
	fclose(f);

	for (int k = 0; k < times; k++) {
		for (int i = 0; i < num_lines; i++) {
			long move_from_pos = original_order[i];
			long move_to_pos = (move_from_pos + mod(dynamic_buffer[move_from_pos], num_lines - 1)) % (num_lines - 1);

			long number = dynamic_buffer[move_from_pos];
			int pos = backlink[move_from_pos];
			long sign = SIGN(move_to_pos - move_from_pos);
			for (int k = move_from_pos; k != move_to_pos; k += sign) {
				dynamic_buffer[k] = dynamic_buffer[k + sign];
				backlink[k] = backlink[k + sign];
				original_order[backlink[k]] = k;
			}
			backlink[move_to_pos] = pos;
			dynamic_buffer[move_to_pos] = number;
			original_order[pos] = move_to_pos;
		}
	}

	for (int i = 0; i < num_lines; i++) {
		if (dynamic_buffer[i] == 0) {
			signed long coord1 = dynamic_buffer[(i + 1000) % num_lines];
			signed long coord2 = dynamic_buffer[(i + 2000) % num_lines];
			signed long coord3 = dynamic_buffer[(i + 3000) % num_lines];
			printf("Coords: %ld, %ld, %ld --> sum: %ld\n", coord1, coord2, coord3, coord1 + coord2 + coord3);
		}
	}
}
