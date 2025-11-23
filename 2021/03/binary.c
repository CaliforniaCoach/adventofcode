#include <stdio.h>
#include <string.h>
#include <assert.h>

int filter(int numbers[], int count, int num_digits, int most_common) {
	int numbers_copy[count];
	memcpy(numbers_copy, numbers, count * sizeof(*numbers));
	numbers = numbers_copy;
	for (int digit = num_digits - 1; count > 1; digit--) {
		assert(digit >= 0);
		int one_count = 0;
		for (int number = 0; number < count; number++) {
			one_count += numbers[number] >> digit & 1;
		}
		int bit_criteria = most_common ? one_count >= (count + 1) / 2 : one_count < (count + 1) / 2;
		for (int number = 0; number < count; number++) {
			if ((numbers[number] >> digit & 1) != bit_criteria) {
				for (int i = number; i < count - 1; i++) {
					numbers[i] = numbers[i + 1];
				}
				count--;
				number--;
			}
		}
	}
	return numbers[0];
}

int main() {
	int numbers[10000] = {0};
	int digit_count[20] = {0};
	int digit = 0;
	int len = -1;
	int count = 0;
	FILE* fp = fopen("input.txt", "r");
	while (1) {
		int chr = fgetc(fp);
		if (feof(fp)) {
			break;
		}
		if (chr == '0' || chr == '1') {
			digit_count[digit++] += chr - '0';
			numbers[count] = numbers[count] << 1 | (chr - '0');
		} else if (chr == '\n') {
			if (len < 0) {
				len = digit;
			} else {
				assert(len == digit);
			}
			digit = 0;
			count++;
		} else {
			assert(0);
		}
	}
	fclose(fp);
	int gamma = 0;
	int epsilon = 0;
	for (int digit = 0; digit < len; digit++) {
		gamma <<= 1;
		epsilon <<= 1;
		if (digit_count[digit] > count / 2) {
			gamma |= 1;
		} else {
			epsilon |= 1;
		}
	}
	printf("%ld\n", (long) gamma * epsilon);

	printf("%ld\n", (long) filter(numbers, count, len, 1) * filter(numbers, count, len, 0));
}
