#include <stdio.h>

int main() {
	int pos[2], score[2] = {0};
	FILE* fp = fopen("input.txt", "r");
	fscanf(fp, "Player 1 starting position: %d\n", pos + 0);
	fscanf(fp, "Player 2 starting position: %d\n", pos + 1);
	fclose(fp);

	int die = 1;
	int player = 0;
	int rolls = 0;
	while (score[0] < 1000 && score[1] < 1000) {
		for (int i = 0; i < 3; i++) {
			pos[player] = (pos[player] + die - 1) % 10 + 1;
			die = (die + 1 - 1) % 100 + 1;
			rolls++;
		}
		score[player] += pos[player];
		player = (player + 1) % 2;
	}
	printf("%d\n", (score[0] < 1000 ? score[0] : score[1]) * rolls);
}
