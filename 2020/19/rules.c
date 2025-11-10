#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>


#define FILENAME "input.txt"


struct Rule {
	enum {RULE, MATCH} type;
	int data;
};

char* match(char* line, struct Rule rules[][10][10], int rule) {
	for (int alt = 0; rules[rule][alt][0].data >= 0; alt++) {
		switch (rules[rule][alt][0].type) {
			case MATCH:
				return rules[rule][alt][0].data == *line ? line + 1 : NULL;

			case RULE:
				char *ptr = line;
				for (int i = 0; rules[rule][alt][i].data >= 0; i++) {
					ptr = match(ptr, rules, rules[rule][alt][i].data);
					if (ptr == NULL) {
						break;
					}
				}
				if (ptr != NULL) {
					return ptr;
				}
				break;

			default:
				assert(0);
		}
	}
	return NULL;
}

char* match2(char* line, struct Rule rules[][10][10]) {
	assert(rules[0][0][0].type == RULE);
	assert(rules[0][0][0].data == 8);
	assert(rules[0][0][1].data == 11);
	assert(rules[0][0][2].data < 0);
	assert(rules[0][1][0].data < 0);
	for (int repetitions_42 = 2;; repetitions_42++) {
		char* ptr = line;
		for (int i = 0; i < repetitions_42; i++) {
			ptr = match(ptr, rules, 42);
			if (ptr == NULL) {
				return NULL;
			}
		}
		for (int k = 0; k < repetitions_42 - 1; k++) {
			ptr = match(ptr, rules, 31);
			if (ptr == NULL) {
				break;
			}
			if (*ptr == '\n' || *ptr == '\0') {
				return ptr;
			}
		}
	}
}


int main() {
	struct Rule rules[1000][10][10];
	memset(rules, -1, sizeof(rules));
	char line[500];
	FILE* fp = fopen(FILENAME, "r");
	while (1) {
		fgets(line, 500, fp);
		char* colon = strchr(line, ':');
		if (colon == NULL) {
			 break;
		}
		int rule_no = atoi(line);
		if (*(colon + 2) == '"') {
			rules[rule_no][0][0].type = MATCH;
			rules[rule_no][0][0].data = *(colon + 3);
		} else {
			char* ptr = colon;
			int alternative = 0;
			int item = 0;
			while(*(++ptr) != '\0') {
				if (*ptr >= '0' && *ptr <= '9') {
					rules[rule_no][alternative][item].type = RULE;
					rules[rule_no][alternative][item].data = atoi(ptr);
					while (*ptr >= '0' && *ptr <= '9') {
						ptr++;
					}
					item++;
				} else if (*ptr == '|') {
					alternative++;
					item = 0;
				} else if (*ptr == ' ') {
					// do nothing
				} else {
					assert(0);
				}
			}
		}
	}

	char msgs[1000][1000];
	int num_msgs = 0;
	while (!feof(fp)) {
		fgets(msgs[num_msgs++], 500, fp);
	}
	fclose(fp);

	int valid1 = 0;
	for (int i = 0; i < num_msgs; i++) {
		char* ptr = match(msgs[i], rules, 0);
		if (ptr != NULL && *ptr == '\n') {
			valid1++;
		}
	}
	printf("Valid messages without loops: %d\n", valid1);


	int valid2 = 0;
	for (int i = 0; i < num_msgs; i++) {
		char* ptr = match2(msgs[i], rules);
		if (ptr != NULL && *ptr == '\n') {
			valid2++;
		}
	}
	printf("Valid messages with loops   : %d\n", valid2);
}
