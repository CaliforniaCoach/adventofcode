#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define LINEBUFSIZE 200

char buf[LINEBUFSIZE];
char baglist[1000][100] = {0};
int children_color[1000][100];
int children_count[1000][100];

int get_bagname_index(char bagname[]) {
	int i;
	for (i = 0; i < 1000 && baglist[i][0] != '\0'; i++) {
		if (!strcmp(baglist[i], bagname)) {
			return i;
		}
	}
	strcpy(baglist[i], bagname);
	return i;
}

int contains(int color_to_search, int contain_color) {
	int does_contain = 0;
	for (int i = 0; children_color[color_to_search][i] >= 0; i++) {
		if (children_color[color_to_search][i] == contain_color) {
			return 1;
		} else {
			if (contains(children_color[color_to_search][i], contain_color)) {
				return 1;
			}
		}
	}
	return 0;
}

long count_bags(int col) {
	long sum = 0;
	for (int i = 0; children_color[col][i] >= 0; i++) {
		sum += children_count[col][i] * (1 + count_bags(children_color[col][i]));
	}
	return sum;
}

int main() {
	FILE* fp = fopen("input.txt", "r");
	while (fgets(buf, LINEBUFSIZE, fp)) {
		char* p = strstr(buf, " bags contain ");
		*p = '\0';
		int parent = get_bagname_index(buf);
		p += strlen(" bags contain ");
		int i = 0;
		while (*p != '\0') {
			if (!strncmp(p, "no other bags.", strlen("no other bags."))) {
				break;
			}
			int num = atoi(p);
			assert(num > 0);
			while ((*(++p) >= '0' && *p <= '9') || *p == ' ');
			char* p2 = strstr(p, num == 1 ? " bag" : " bags");
			*p2 = '\0';
			int color = get_bagname_index(p);
			children_color[parent][i] = color;
			children_count[parent][i++] = num;
			p = p2 + (num == 1 ? 6 : 7);
		}
		children_color[parent][i] = -1;
	}
	fclose(fp);

	int shiny_gold = get_bagname_index("shiny gold");
	int count = 0;
	for (int i = 0; baglist[i][0] != '\0'; i++) {
		count += contains(i, shiny_gold);
	}
	printf("%d\n", count);
	printf("%ld\n", count_bags(shiny_gold));
}
