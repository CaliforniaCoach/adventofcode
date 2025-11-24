#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

int num_common_chars(char str1[], char str2[]) {
	int count = 0;
	while (*str1 != '\0') {
		if (strchr(str2, *(str1++)) != NULL) {
			count++;
		}
	}
	return count;
}

int comp(const void* c1, const void* c2) {
	return *(char*)c1 - *(char*)c2;
}

int main() {
	char map[1000][10][10];
	char output[1000][4][10];
	int num_outputs = 0;
	FILE* fp = fopen("input.txt", "r");
	while (!feof(fp)) {
		for (int i = 0; i < 10; i++) {
			if (!fscanf(fp, "%s", map[num_outputs][i])) {
				break;
			}
			qsort(map[num_outputs][i], strlen(map[num_outputs][i]), sizeof(char), comp);
		}
		fscanf(fp, " | ");
		for (int i = 0; i < 4; i++) {
			if (!fscanf(fp, "%s", output[num_outputs][i])) {
				break;
			}
			qsort(output[num_outputs][i], strlen(output[num_outputs][i]), sizeof(char), comp);
		}
		fscanf(fp, " ");
		num_outputs++;
	}
	fclose(fp);

	int count = 0;
	int sum = 0;
	for (int line = 0; line < num_outputs; line++) {
		char* number_map[10];
		for (int pattern = 0; pattern < 10; pattern++) {
			switch (strlen(map[line][pattern])) {
				case 2:
					number_map[1] = map[line][pattern];
					break;
				case 3:
					number_map[7] = map[line][pattern];
					break;
				case 4:
					number_map[4] = map[line][pattern];
					break;
				case 7:
					number_map[8] = map[line][pattern];
					break;
			}
		}
		for (int pattern = 0; pattern < 10; pattern++) {
			switch (strlen(map[line][pattern])) {
				// 5 segments can be 2, 3, 5
				case 5:
					if (num_common_chars(map[line][pattern], number_map[1]) == 2) {
						number_map[3] = map[line][pattern];
					} else if (num_common_chars(map[line][pattern], number_map[4]) == 3) {
						number_map[5] = map[line][pattern];
					} else {
						number_map[2] = map[line][pattern];
					}
					break;
				// 6 segments can be 6, 9, 0
				case 6:
					if (num_common_chars(map[line][pattern], number_map[1]) == 1) {
						number_map[6] = map[line][pattern];
						break;
					} else if (num_common_chars(map[line][pattern], number_map[4]) == 4) {
						number_map[9] = map[line][pattern];
					} else {
						number_map[0] = map[line][pattern];
					}
					break;
			}
		}
		int number = 0;
		for (int out = 0; out < 4; out++) {
			switch (strlen(output[line][out])) {
				case 2:
				case 3:
				case 4:
				case 7:
					count++;
					break;
			}
			int number_found = 0;
			number *= 10;
			for (int digit = 0; digit < 10; digit++) {
				if (!strcmp(number_map[digit], output[line][out])) {
					number += digit;
					number_found = 1;
					break;
				}
			}
			assert(number_found);
		}
		sum += number;
	}
	printf("%d\n", count);
	printf("%d\n", sum);
}
