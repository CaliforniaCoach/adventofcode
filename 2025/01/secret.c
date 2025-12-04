#include <stdio.h>
#include <assert.h>

#define MOD(a, b) (((b) + (a) % (b)) % (b))

int main() {
	int pos = 50;
	int num_zero_positions1 = 0;
	int num_zero_positions2 = 0;
	FILE* fp = fopen("input.txt", "r");
	char dir;
	int amount;
	while (fscanf(fp, " %c%d ", &dir, &amount) != EOF) {
		int temp = pos + (dir == 'R' ? 1 : -1) * amount;
		if (temp > 0) {
			num_zero_positions2 += temp / 100;
		} else if (pos != 0) {
			 num_zero_positions2 += (-temp + 100) / 100;
		} else {
			 num_zero_positions2 += -temp / 100;
		}
		pos = MOD(temp, 100);
		assert(pos >= 0 && pos <= 99);
		if (pos == 0) {
			num_zero_positions1++;
		}
	}
	fclose(fp);
	printf("%d\n", num_zero_positions1);
	printf("%d\n", num_zero_positions2);
}
