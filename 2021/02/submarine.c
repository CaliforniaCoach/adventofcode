#include <stdio.h>
#include <string.h>
#include <assert.h>

int main() {
	int pos1 = 0;
	int depth1 = 0;
	int pos2 = 0;
	int depth2 = 0;
	int aim = 0;
	FILE* fp = fopen("input.txt", "r");
	while (1) {
		char cmd[10];
		int arg;
		if(fscanf(fp, "%s %d", cmd, &arg) != 2) {
			break;
		}
		if (!strcmp(cmd, "forward")) {
			pos1 += arg;
			pos2 += arg;
			depth2 += arg * aim;
		} else if (!strcmp(cmd, "down")) {
			depth1 += arg;
			aim += arg;
		} else if(!strcmp(cmd, "up")) {
			depth1 -= arg;
			aim -= arg;
		} else {
			assert(0);
		}
		assert(depth1 >= 0);
		assert(depth2 >= 0);
	}
	fclose(fp);
	printf("Part 1: %d\n", depth1 * pos1);
	printf("Part 2: %d\n", depth2 * pos2);
}
