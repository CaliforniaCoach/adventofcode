#include <stdlib.h>
#include <stdio.h>
#include <string.h>


const int MAX_LINE_LENGTH = 1000;

const int MAX_RED = 12;
const int MAX_GREEN = 13;
const int MAX_BLUE = 14;


#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))


int main(int argc, char** argv) {

	FILE* f = fopen("input.txt", "r");
	char line[MAX_LINE_LENGTH];

	int sum = 0;
	long sum2 = 0;

	while (fgets(line, MAX_LINE_LENGTH, f) != NULL) {
		if (strncmp(line, "Game ", 5)) {
			return 1;
		}

		int id = atoi(line + 5);

		int min_red = 0, min_green = 0, min_blue = 0;
		int impossible = 0;

		char* ptr = strchr(line, ':') + 2;
		while (*(ptr - 2) != '\n' && *(ptr - 1) != '\0')  {
			int num = atoi(ptr);
			ptr = strchr(ptr, ' ') + 1;
			if (!strncmp(ptr, "red", 3)) {
				min_red = MAX(min_red, num);
				if (num > MAX_RED) {
					impossible = 1;
				}
				ptr += 3;
			}
			if (!strncmp(ptr, "green", 5)) {
				min_green = MAX(min_green, num);
				if (num > MAX_GREEN) {
					impossible = 1;
				}
				ptr += 5;
			}
			if (!strncmp(ptr, "blue", 4)) {
				min_blue = MAX(min_blue, num);
				if (num > MAX_BLUE) {
					impossible = 1;
				}
				ptr += 4;
			}
			ptr += 2;
		}
		sum += impossible ? 0 : id;
		sum2 += min_red * min_green * min_blue;
	}

	printf("Sum: %d\n", sum);
	printf("Sum2: %d\n", sum2);
}
