#include <stdio.h>
#include <string.h>

int count_yes(int answers[]) {
	int count = 0;
	for (int i = 0; i < 26; i++) {
		count += answers[i] ? 1 : 0;
	}
	return count;
}

int count_yes2(unsigned long bitfield) {
	int count = 0;
	while (bitfield) {
		count += bitfield & 1;
		bitfield >>= 1;
	}
	return count;
}

int main() {
	int question[26] = {0};
	unsigned long question2first = 0L;
	unsigned long question2subsequent = 0L;
	FILE* fp = fopen("input.txt", "r");
	char chr;
	int newline_counter = 0;
	int yes_counter = 0, yes_counter2 = 0;
	int first_person = 1;
	while ((chr = fgetc(fp)) != EOF) {
		if (chr == '\n') {
			if (++newline_counter <= 1) {
				if (!first_person) {
					question2first &= question2subsequent;
				}
				question2subsequent = 0L;
				first_person = 0;
			} else {
				yes_counter += count_yes(question);
				yes_counter2 += count_yes2(question2first);
				memset(question, 0, 26 * sizeof(int));
				question2first = 0L;
				first_person = 1;
				newline_counter = 0;
			}
		} else {
			newline_counter = 0;
			question[chr - 'a'] = 1;
			if (first_person) {
				question2first |= 1 << (chr - 'a');
			} else {
				question2subsequent |= 1 << (chr - 'a');
			}
		}
	}
	fclose(fp);
	yes_counter += count_yes(question);
	yes_counter2 += count_yes2(question2first);

	printf("%d\n", yes_counter);
	printf("%d\n", yes_counter2);
}
