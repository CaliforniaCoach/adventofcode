#include <stdio.h>
#include <stdlib.h>

#define ABS(x) ((x) >= 0 ? (x) : -(x))

char command[1000];
int param[1000];
int num_commands = 0;

int run1() {
	int x = 0, y = 0, dx = 1, dy = 0;
	for (int i = 0; i < num_commands; i++) {
		switch (command[i]) {
			case 'N':
				y += param[i];
				break;
			case 'S':
				y -= param[i];
				break;
			case 'E':
				x += param[i];
				break;
			case 'W':
				x -= param[i];
				break;
			case 'L':
			case 'R':
				switch (param[i]) {
					case 0:
						break;
					case 90:
						int dx2 = dx;
						dx = command[i] == 'L' ? -dy : dy;
						dy = command[i] == 'L' ? dx2 : -dx2;
						break;
					case 180:
						dx = -dx;
						dy = -dy;
						break;
					case 270:
						dx2 = dx;
						dx = command[i] == 'L' ? dy : -dy;
						dy = command[i] == 'L' ? -dx2 : dx2;
						break;
					default:
						exit(1);
				}
				break;
			case 'F':
				x += dx * param[i];
				y += dy * param[i];
				break;
			default:
				exit(1);
		}
	}
	return ABS(x) + ABS(y);
}

int run2() {
	int x = 0, y = 0, dx = 10, dy = 1;
	for (int i = 0; i < num_commands; i++) {
		switch (command[i]) {
			case 'N':
				dy += param[i];
				break;
			case 'S':
				dy -= param[i];
				break;
			case 'E':
				dx += param[i];
				break;
			case 'W':
				dx -= param[i];
				break;
			case 'L':
			case 'R':
				switch (param[i]) {
					case 0:
						break;
					case 90:
						int dx2 = dx;
						dx = command[i] == 'L' ? -dy : dy;
						dy = command[i] == 'L' ? dx2 : -dx2;
						break;
					case 180:
						dx = -dx;
						dy = -dy;
						break;
					case 270:
						dx2 = dx;
						dx = command[i] == 'L' ? dy : -dy;
						dy = command[i] == 'L' ? -dx2 : dx2;
						break;
					default:
						exit(1);
				}
				break;
			case 'F':
				x += dx * param[i];
				y += dy * param[i];
				break;
			default:
				exit(1);
		}
	}
	return ABS(x) + ABS(y);
}


int main() {
	char buf[100];
	FILE* fp = fopen("input.txt", "r");
	while (!feof(fp)) {
		fscanf(fp, "%c%d ", command + num_commands, param + num_commands);
		num_commands++;
	}
	fclose(fp);

	printf("%d\n", run1());
	printf("%d\n", run2());
}
