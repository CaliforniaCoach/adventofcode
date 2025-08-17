#include <stdio.h>
#include <string.h>

int count_trees(char map[][100], int num_rows, int right_per_step, int down_per_step) {
	int tree_count = 0;
	int x = 0;
	for (int row = 0; row < num_rows; row += down_per_step) {
		if (map[row][x] == '#') {
			tree_count++;
		}
		x = (x + right_per_step) % strlen(map[row]);
	}
	return tree_count;
}

int main() {
	char map[1000][100];
	int row = 0;
	FILE* fp = fopen("input.txt", "r");
	while (fgets(map[row], 100, fp)) {
		map[row][strlen(map[row]) - 1] = '\0';
		row++;
	}
	fclose(fp);
	int num_rows = row;

	int trees = count_trees(map, num_rows, 3, 1);
	printf("%d\n", trees);

	long product = trees;
	product *= count_trees(map, num_rows, 1, 1);
	product *= count_trees(map, num_rows, 5, 1);
	product *= count_trees(map, num_rows, 7, 1);
	product *= count_trees(map, num_rows, 1, 2);
	printf("%ld\n", product);
}
