#include <stdio.h>

int main(int argc, char** argv) {
	int time[10];
	int distance[10];
	int race = 0;

	FILE* fp = fopen("input.txt", "r");

	fscanf(fp, "%*s");
	int i = 0;
	while(fscanf(fp, "%d", time +  i++));
	int num_times = i - 1;

	fscanf(fp, "%*s");
	i = 0;
	while(!feof(fp) && fscanf(fp, "%d", distance +  i++));
	int num_distances = i - 1;

	fclose(fp);

	int mult = 1;
	for (int ti = 0; ti < num_times; ti++) {
		int win_counter = 0;
		for (int button_time = 0; button_time < time[ti]; button_time++) {
			int dist = (time[ti] - button_time) * button_time;
			if (dist > distance[ti]) {
				win_counter++;
			}
		}
		mult *= win_counter;
	}

	printf("%d\n", mult);

	/////////////////// Part 2 ////////////////////
	long one_time = 0;
	for (int ti = 0; ti < num_times; ti++) {
		long ten_potence = 1;
		while (time[ti] >= ten_potence) {
			ten_potence *= 10;
		}
		one_time = one_time * ten_potence + time[ti];
	}

	long one_dist = 0;
	for (int ti = 0; ti < num_times; ti++) {
		long ten_potence = 1;
		while (distance[ti] >= ten_potence) {
			ten_potence *= 10;
		}
		one_dist = one_dist * ten_potence + distance[ti];
	}

	long win_counter = 0;
	for (long button_time = 0; button_time < one_time; button_time++) {
		long dist = (one_time - button_time) * button_time;
		if (dist > one_dist) {
			win_counter++;
		}
	}
	printf("%ld %ld\n", one_time, one_dist);
	printf("%d\n", win_counter);
}
