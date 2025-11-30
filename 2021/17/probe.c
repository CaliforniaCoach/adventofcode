#include <stdio.h>

int main() {
	FILE* fp = fopen("input.txt", "r");
	int xmin, xmax, ymin, ymax;
	fscanf(fp, "target area: x = %d .. %d , y = %d .. %d", &xmin, &xmax, &ymin, &ymax);
	fclose(fp);

	// When probe falls down, it will reach y = 0 again with inverse initial velocity, i.e. -vy0
	// Then, in the next step, it will fall to y position -vy0. This must not be below ymin,
	// otherwise we will overshoot below target area. So vy0 = -ymin is max possible value for vy0.
	int vy0 = -ymin;

	// Highest y is vy0 + (vy0 - 1) + (vy0 - 2) + ... 1 + 0  ==  1 + 2 + 3 + ... + vy0
	int highest_y = vy0 * (vy0 - 1) / 2;

	printf("%d\n", highest_y);

	int count = 0;
	for (int vx0 = 1; vx0 <= xmax; vx0++) {
		for (int vy0 = ymin; vy0 <= -ymin; vy0++) {
			for (int step = 1, vx = vx0, vy = vy0, x = 0, y = 0; x <= xmax && y >= ymin; step++) {
				x += vx;
				vx = vx > 0 ? vx - 1 : 0;
				y += vy--;
				if (x >= xmin && x <= xmax && y >= ymin && y <= ymax) {
					count++;
					break;
				}
			}
		}
	}
	printf("%d\n", count);
}
