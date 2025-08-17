#include <stdio.h>

int main() {
	int valid = 0;
	int valid2 = 0;
	char buf[1000];
	FILE* fp = fopen("input.txt", "r");
	while (!feof(fp)) {
		char pw[1000];
		char chr;
		int min, max;
		fscanf(fp, "%d-%d %c: %s\n", &min, &max, &chr, pw);
		int count = 0;
		for (char* p = pw; *p != '\0'; p++) {
			if (*p == chr) {
				count++;
			}
		}
		if (count >= min && count <= max) {
			valid++;
		}
		if ((pw[min - 1] == chr) != (pw[max - 1] == chr)) {
			valid2++;
		}
	}
	fclose(fp);
	printf("%d\n", valid);
	printf("%d\n", valid2);
}
