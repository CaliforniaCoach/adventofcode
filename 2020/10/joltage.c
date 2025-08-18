#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>

int comp(const void* e1, const void* e2) {
	int* p1 = (int*) e1;
	int* p2 = (int*) e2;
	int i1 = *p1;
	int i2 = *p2;
	return i1 - i2;
}

void add_to_diff_counter(int diff_count[], int diff) {
	assert(diff >= 1 && diff <= 3);
	diff_count[diff]++;
}

int main() {
	int adapters[500];
	char buf[100];
	int num_adapters = 0;
	FILE* fp = fopen("input.txt", "r");
	while (fgets(buf, 100, fp)) {
		adapters[num_adapters++] = atoi(buf);
	}
	fclose(fp);

	qsort(adapters, num_adapters, sizeof(*adapters), comp);

	int diff_count[4] = {0};
	for (int i = 0; i < num_adapters - 1; i++) {
		add_to_diff_counter(diff_count, adapters[i + 1] - adapters[i]);
	}
	add_to_diff_counter(diff_count, adapters[0] - 0);
	add_to_diff_counter(diff_count, 3);

	printf("%d\n", diff_count[1] * diff_count[3]);

	unsigned long combinations[num_adapters];
	for (int i = 0; i < num_adapters; i++) {
		combinations[i] = adapters[i] <= 3 ? 1 : 0;
		for (int k = i - 1; k >= 0; k--) {
			if (adapters[i] - adapters[k] > 3) {
				break;
			}
			if (combinations[k] > LONG_MAX - combinations[i]) {
				// Overflow!
				exit(1);
			}
			combinations[i] += combinations[k];
		}
	}

	printf("%ld\n", combinations[num_adapters - 1]);
}
