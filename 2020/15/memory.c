#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define INPUT 0,1,5,10,3,12,19
// #define INPUT 0,3,6

void run1(unsigned int starting_numbers[], unsigned int num_starting_numbers, unsigned int len) {
	unsigned int* numbers = malloc(len * sizeof(unsigned int));
	memcpy(numbers, starting_numbers, num_starting_numbers * sizeof(unsigned int));

	for (unsigned int i = num_starting_numbers; i < len; i++) {
		unsigned int last = numbers[i - 1];
		numbers[i] = 0;
		for (int k = (int) i - 2; k >= 0; k--) {
 			if (numbers[k] == last) {
				numbers[i] = i - 1 - (unsigned int) k;
				break;
			}
		}
	}
	printf("%d\n", numbers[len - 1]);
	free(numbers);
}

void run2(unsigned int initial_numbers[], unsigned int initial_numbers_len, unsigned int len) {
	int* last_idx = malloc(len * sizeof(int));
    memset(last_idx, -1, len * sizeof(*last_idx));
    for (unsigned int i = 0; i < initial_numbers_len; i++) {
        last_idx[initial_numbers[i]] = (int) i;
    }
    unsigned int last_num = initial_numbers[initial_numbers_len - 1];
    int last_idx_last_num = -1;
    for (unsigned int i = initial_numbers_len; i < len; i++) {
        unsigned int next_num = 0;
        if (last_idx_last_num >= 0) {
            next_num = i - (unsigned int) last_idx_last_num - 1;
        }
        last_idx_last_num = last_idx[next_num];
        last_idx[next_num] = (int) i;
        last_num = next_num;
    }
    printf("%d\n", last_num);
	free(last_idx);
}

int main() {
	unsigned int starting_numbers[] = {INPUT};
	run1(starting_numbers, sizeof(starting_numbers) / sizeof(*starting_numbers), 2020);
	run2(starting_numbers, sizeof(starting_numbers) / sizeof(*starting_numbers), 30000000);
}
