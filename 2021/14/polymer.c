#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <assert.h>

#define PAIR_INDEX(c1, c2) (((c1) - 'A') << 5 | ((c2) - 'A'))

long long solve(const char initial_polymer[], const char map[], const __uint16_t pairs[],
				int num_pairs, int steps) {
	long long frequency[1 << 10] = {0};
	for (const char* ptr = initial_polymer; *(ptr + 1) != '\0'; ptr++) {
		frequency[PAIR_INDEX(*ptr, *(ptr + 1))]++;
	}
	for (int i = 0; i < steps; i++) {
		long long frequency2[1 << 10] = {0};
		memcpy(frequency2, frequency, sizeof(frequency));
		for (int k = 0; k < num_pairs; k++) {
			int pair_idx1 = (pairs[k] & 0b1111100000) | map[pairs[k]];
			int pair_idx2 = (map[pairs[k]] << 5) | (pairs[k] & 0b0000011111);
			assert(map[pair_idx1] >= 0 && map[pair_idx2] >= 0);
			assert(frequency2[pair_idx1] < LLONG_MAX - frequency[pairs[k]]);
			assert(frequency2[pair_idx2] < LLONG_MAX - frequency[pairs[k]]);
			assert(frequency2[pairs[k]] > LLONG_MIN + frequency[pairs[k]]);
			frequency2[pair_idx1] += frequency[pairs[k]];
			frequency2[pair_idx2] += frequency[pairs[k]];
			frequency2[pairs[k]] -= frequency[pairs[k]];
		}
		memcpy(frequency, frequency2, sizeof(frequency));
	}

	long long max_count = 0;
	long long min_count = LLONG_MAX;
	for (char atom = 0; atom <= 'Z' - 'A'; atom++) {
		long long count = 0;
		for (int i = 0; i < num_pairs; i++) {
			assert(frequency[pairs[i]] >= 0);
			count += atom == pairs[i] >> 5 ? frequency[pairs[i]] : 0;
			count += atom == (pairs[i] & 0b11111) ? frequency[pairs[i]] : 0;
			assert(count >= 0LL);
		}
		count = (count + 1) / 2;
		if (count > 0) {
			max_count = count > max_count ? count : max_count;
			min_count = count < min_count ? count : min_count;
		}
	}
	return max_count - min_count;
}

int main() {
	char initial_polymer[100000] = {};
	char map[1 << 10];
	__uint16_t pairs[1000] = {0};
	int num_pairs = 0;
	memset(map, -1, sizeof(map));
	FILE* fp = fopen("input.txt", "r");
	fgets(initial_polymer, 100000, fp);
	initial_polymer[strlen(initial_polymer) - 1] = '\0';
	fseek(fp, 1, SEEK_CUR);
	while (1) {
		int c1 = fgetc(fp);
		int c2 = fgetc(fp);
		if (feof(fp)) {
			break;
		}
		fseek(fp, 4, SEEK_CUR);
		int c = fgetc(fp);
		map[PAIR_INDEX(c1, c2)] = c - 'A';
		pairs[num_pairs++] = PAIR_INDEX(c1, c2);
		fseek(fp, 1, SEEK_CUR);
	}
	fclose(fp);

	printf("%lld\n", solve(initial_polymer, map, pairs, num_pairs, 10));
	printf("%lld\n", solve(initial_polymer, map, pairs, num_pairs, 40));
}
