#include <stdio.h>
#include <assert.h>

#define SWAP(a, b) {int temp = a; a = b; b = temp;}
#define ABS(a) ((a) > 0 ? (a) : -(a))
#define SGN(a) ((a) > 0 ? 1 : ((a) < 0 ? -1 : 0))

int main() {
	int covered1[1000][1000] = {0};
	int covered2[1000][1000] = {0};
	FILE* fp = fopen("input.txt", "r");
	int x1, y1, x2, y2;
	while (fscanf(fp, "%d,%d -> %d,%d", &x1, &y1, &x2, &y2) >= 4) {
		assert(x1 >= 0 && x1 < 1000 && x2 >= 0 && x2 < 1000);
		assert(y1 >= 0 && y1 < 1000 && y2 >= 0 && y2 < 1000);
		if (x1 == x2 || y1 == y2) {
			if (x2 < x1) {
				SWAP(x1, x2);
			}
			if (y2 < y1) {
				SWAP(y1, y2);
			}
			for (int x = x1; x <= x2; x++) {
				for (int y = y1; y <= y2; y++) {
					covered1[x][y]++;
					covered2[x][y]++;
				}
			}
		} else {
			assert(ABS(y2 - y1) == ABS(x2 - x1));
			for (int i = 0; i <= ABS(x2 - x1); i++) {
				covered2[x1 + i * SGN(x2 - x1)][y1 + i * SGN(y2 - y1)]++;
			}
		}
	}
	fclose(fp);

	long count1 = 0;
	long count2 = 0;
	for (int x = 0; x < 1000; x++) {
		for (int y = 0; y < 1000; y++) {
			if (covered1[x][y] > 1) {
				count1++;
			}
			if (covered2[x][y] > 1) {
				count2++;
			}
		}
	}
	printf("%ld\n%ld\n", count1, count2);
}
