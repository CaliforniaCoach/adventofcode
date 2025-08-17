#include <stdio.h>
#include <stdlib.h>

long numbers[1500];

int main() {
	char buf[100];
	FILE* fp = fopen("input.txt", "r");
	int row = 0;
	while (fgets(buf, 100, fp)) {
		numbers[row++] = atol(buf);
	}
	fclose(fp);

	long invalid_num;
	for (int i = 25; i < row; i++) {
		long num = numbers[i];
		int hit = 0;
		for (int k = i - 25; k < i; k++) {
			long num1 = numbers[k];
			for (int m = k + 1; m < i; m++) {
				long num2 = numbers[m];
				if (num1 + num2 == num) {
					hit = 1;
					break;
				}
			}
			if (hit) {
				break;
			}
		}
		if (!hit) {
			printf("Part 1 solution: %d\n", num);
			invalid_num = num;
		}
	}

	for (int i = 0; i < row; i++) {
		long sum = numbers[i];
		long min = sum;
		long max = sum;
		for (int k = i + 1; sum < invalid_num && k < row; k++) {
			long num = numbers[k];
			sum += num;
			if (num < min) min = num;
			if (num > max) max = num;
			if (sum == invalid_num) {
				printf("Part 2 solution: %d\n", min + max);
			}
		}
	}
}
