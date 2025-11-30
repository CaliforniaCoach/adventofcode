#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

enum Result {CORRUPT, INCOMPLETE, OK};

int compare(const void* a, const void* b) {
	long diff = *(long*)a - *(long*)b;
	return diff > 0 ? 1 : diff < 0 ? -1 : 0;
}

int main() {
	int score1 = 0;
	long score2[1000];
	int num_score2 = 0;
	FILE* fp = fopen("input.txt", "r");
	while (!feof(fp)) {
		char closing_chr[100];
		int level = 0;
		int chr;
		int count = 0;
		enum Result result;
		while (1) {
			chr = getc(fp);
			if (chr < 0 || chr == '\n') {
				result = level == 0 ? OK : INCOMPLETE;
				break;
			} else if (chr == '(') {
				closing_chr[level++] = ')';
			} else if (chr == '[') {
				closing_chr[level++] = ']';
			} else if (chr == '{') {
				closing_chr[level++] = '}';
			} else if (chr == '<') {
				closing_chr[level++] = '>';
			} else if (chr == ')' || chr == ']' || chr == '}' || chr == '>') {
				if (--level < 0 || chr != closing_chr[level]) {
					result = CORRUPT;
					break;
				}
			} else {
				assert(0);
			}
			count++;
		}
		if (result == CORRUPT) {
			switch (chr) {
				case ')': score1 += 3; break;
				case ']': score1 += 57; break;
				case '}': score1 += 1197; break;
				case '>': score1 += 25137; break;
				default: assert(0);
			}
			do {
				chr = getc(fp);
			} while (chr != '\n' && chr != -1);
		} else if (result == INCOMPLETE) {
			long line_score2 = 0;
			while (--level >= 0) {
				assert(__LONG_MAX__ / 5 - 4 > line_score2);
				line_score2 *= 5;
				switch (closing_chr[level]) {
					case ')': line_score2 += 1; break;
					case ']': line_score2 += 2; break;
					case '}': line_score2 += 3; break;
					case '>': line_score2 += 4; break;
					default: assert(0);
				}
			}
			score2[num_score2++] = line_score2;
		}
	}
	fclose(fp);

	printf("%d\n", score1);

	qsort(score2, num_score2, sizeof(*score2), compare);
	assert(num_score2 % 2 == 1);
	printf("%ld\n", score2[num_score2 / 2]);
}
