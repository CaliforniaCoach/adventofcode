#include <stdio.h>
#include <string.h>

char room[100][100];
char room2[100][100];
char room_backup[100][100];
int rows = 0;

int step(int part) {
	int changed = 0;
	for (int y = 0; y < rows; y++) {
		for (int x = 0; room[y][x] != '\n'; x++) {
			int neighbors = 0;
			for (int dx = -1; dx <= 1; dx++) {
				for (int dy = -1; dy <= 1; dy++) {
					if (dx || dy) {
						int x2 = x + dx;
						int y2 = y + dy;
						while (y2 >= 0 && y2 < rows && x2 >= 0 && room[y2][x2] != '\n') {
							if (room[y2][x2] == '#') {
								neighbors++;
								break;
							}
							if (room[y2][x2] == 'L' || part == 1) {
								break;
							}
							x2 += dx;
							y2 += dy;
						}
					}
				}
			}
			if (room[y][x] == 'L' && neighbors == 0) {
				room2[y][x] = '#';
				changed = 1;
			} else if (room[y][x] == '#' && neighbors >= (part == 1 ? 4 : 5)) {
				room2[y][x] = 'L';
				changed = 1;
			}
		}
	}
	memcpy(room, room2, 100 * 100 * sizeof(char));
	return changed;
}

int count_occupied_seats() {
	int count = 0;
	for (int i = 0; i < 100 * 100; i++) {
		if (*((char*)room + i) == '#') {
			count++;
		}
	}
	return count;
}

void print_room(char room[][100]) {
	for (int y = 0; y < rows; y++) {
		for (int x = 0; room[y][x] != '\n'; x++) {
			putc(room[y][x], stdout);
		}
		puts("");
	}
	puts("");
}

int main() {
	FILE* fp = fopen("input.txt", "r");
	while (fgets(room[rows++], 100, fp));
	fclose(fp);
	rows--;
	memcpy(room2, room, 100 * 100 * sizeof(char));
	memcpy(room_backup, room, 100 * 100 * sizeof(char));

	while (step(1));
	printf("%d\n", count_occupied_seats());

	memcpy(room, room_backup, 100 * 100 * sizeof(char));
	memcpy(room2, room_backup, 100 * 100 * sizeof(char));
	while (step(2));
	printf("%d\n", count_occupied_seats());
}
