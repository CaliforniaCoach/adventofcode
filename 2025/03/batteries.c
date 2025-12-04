#include <stdio.h>
#include <assert.h>

long long find_largest_joltage(char line[], int num_digits) {
	long long joltage = 0;
	char* next_start_pos = line;
	for (int digit = 0; digit < num_digits; digit++) {
		char* highest_pos;
		int highest_digit_val = -1;
		for (
			char* ptr = next_start_pos;
			*(ptr + num_digits - digit - 1) != '\0' && *(ptr + num_digits - digit - 1) != '\n';
			ptr++
		) {
			assert(*ptr >= '0' && *ptr <= '9');
			int digit_val = *ptr - '0';
			if (digit_val > highest_digit_val) {
				highest_digit_val = digit_val;
				highest_pos = ptr;
			}
		}
		joltage = joltage * 10 + highest_digit_val;
		next_start_pos = highest_pos + 1;
	}
	return joltage;
}

int main() {
	long long sum1 = 0;
	long long sum2 = 0;
	FILE* fp = fopen("input.txt", "r");
	char line[1000];
	while (fgets(line, 1000, fp)) {
		sum1 += find_largest_joltage(line, 2);
		sum2 += find_largest_joltage(line, 12);
	}
	fclose(fp);
	printf("%lld\n%lld\n", sum1, sum2);
}
