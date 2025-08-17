#include <stdio.h>
#include <stdlib.h>

int main() {
	int numbers[500];
	char buf[100];
	int i = 0;
	FILE* fp = fopen("input.txt", "r");
	while(fgets(buf, 100, fp)) {
		numbers[i++] = atoi(buf);
	}
	fclose(fp);

	int num_numbers = i;

	for (int i = 0; i < num_numbers; i++) {
		for (int k = i + 1; k < num_numbers; k++) {
			if (numbers[i] + numbers[k] == 2020) {
				printf("Part 1 solution: %d\n", numbers[i] * numbers[k]);
			}
			for (int m = k + 1; m < num_numbers; m++) {
				if (numbers[i] + numbers[k] + numbers[m] == 2020) {
					printf("Part 2 solution: %d\n", numbers[i] * numbers[k] * numbers[m]);
				}
			}
		}
	}
}
