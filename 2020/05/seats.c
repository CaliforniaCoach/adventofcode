#include <stdio.h>

int main() {
	int min_id = 1 << 15;
	int max_id = 0;
	int sum = 0;
	char buf[15];
	FILE* fp = fopen("input.txt", "r");
	while (fgets(buf, 15, fp)) {
		int row = 0;
		int col = 0;
		for (int i = 0; i < 7; i++) {
			row <<= 1;
			if (buf[i] == 'B') {
				row |= 1;
			}
		}
		for (int i = 0; i < 3; i++) {
			col <<= 1;
			if (buf[i + 7] == 'R') {
				col |= 1;
			}
		}
		int id = row * 8 + col;
		if (id > max_id) {
			max_id = id;
		}
		if (id < min_id) {
			min_id = id;
		}
		sum += id;
	}
	fclose(fp);
	printf("%d\n", max_id);

	int n = 127 * 8 - 1;
	printf("%d\n", (max_id + 1) * max_id / 2 - (min_id - 1 + 1) * (min_id - 1) / 2 - sum);
}
