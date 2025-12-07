#include <stdio.h>
#include <string.h>
#include <assert.h>

#define IMG_BUF_SIZE 800
#define MARGIN 200

long process(const int image[][IMG_BUF_SIZE], const int lut[], int num_steps) {
	int buf1[IMG_BUF_SIZE][IMG_BUF_SIZE];
	int buf2[IMG_BUF_SIZE][IMG_BUF_SIZE] = {0};
	memcpy(buf1, image, sizeof(buf1));
	int (*img1)[IMG_BUF_SIZE] = buf1;
	int (*img2)[IMG_BUF_SIZE] = buf2;
	for (int step = 0; step < num_steps; step++) {
		for (int y = 1; y < IMG_BUF_SIZE - 1; y++) {
			for (int x = 1; x < IMG_BUF_SIZE - 1; x++) {
				int input_val = 0;
				for (int dy = -1; dy <= 1; dy++) {
					for (int dx = -1; dx <= 1; dx++) {
						input_val = (input_val << 1) | (img1[y + dy][x + dx] ? 1 : 0);
					}
				}
				img2[y][x] = lut[input_val];
			}
		}
		int (*temp)[IMG_BUF_SIZE] = img1;
		img1 = img2;
		img2 = temp;
	}
	long count = 0;
	for (int y = MARGIN / 2; y < IMG_BUF_SIZE - MARGIN / 2; y++) {
		for (int x = MARGIN / 2; x < IMG_BUF_SIZE - MARGIN / 2; x++) {
			if (img1[y][x]) {
				count++;
			}
		}
	}
	return count;
}

int main() {
	int lut[512];
	int img[IMG_BUF_SIZE][IMG_BUF_SIZE] = {0};
	FILE* fp = fopen("input.txt", "r");
	for (int i = 0; i < 512; i++) {
		lut[i] = fgetc(fp) == '#';
	}
	fgetc(fp);
	fgetc(fp);
	int x = MARGIN;
	int y = MARGIN;
	while (1) {
		int chr = fgetc(fp);
		if (feof(fp)) {
			break;
		}
		if (chr == '#') {
			img[y][x] = 1;
		} else if (chr == '\n') {
			x = MARGIN - 1;
			y++;
		} else {
			assert(chr == '.');
		}
		x++;
	}
	fclose(fp);

	printf("%ld\n%ld\n", process(img, lut, 2), process(img, lut, 50));
}
