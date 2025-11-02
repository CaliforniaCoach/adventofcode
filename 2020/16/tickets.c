#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#define MAX_FIELDS 100

int temp;
#define SWAP(a, b) (temp = (a), (a) = (b), (b) = temp)

struct Rule {
	char name[100];
	int min1, max1, min2, max2;
	int impossible_fields[MAX_FIELDS];
	int field;
};

void extract_numbers(int* min, int* max, char* rule) {
	char* sep = strchr(rule, '-');
	*min = atoi(rule);
	*max = atoi(sep + 1);
}

int parse_ticket(int* ticket, char* line) {
	char* num = line;
	int num_fields = 0;
	while (num != NULL) {
		ticket[num_fields] = atoi(num);
		num = strchr(num, ',');
		if (num != NULL) {
			num++;
		}
		num_fields++;
	}
	return num_fields;
}

int rule_fulfilled(struct Rule* rule, int val) {
	return (val >= rule->min1 && val <= rule->max1) || (val >= rule->min2 && val <= rule->max2);
}

int print_solution(struct Rule rules[], int num_rules, int my_ticket[]) {
	int num = 0;
	long product = 1;
	for (int rule = 0; rule < num_rules; rule++) {
		// Null termination character must be substracted from sizeof("...")
		if (!strncmp(rules[rule].name, "departure", sizeof("departure") / sizeof(char) - 1)) {
			num++;
			product *= my_ticket[rules[rule].field];
		}
	}
	assert(num == 6);
	printf("Solution part 2: %ld\n", product);
}

void permute1(struct Rule rules[], int current_rule, int num_rules, int my_ticket[]) {
	static int field_used[MAX_FIELDS];
	if (current_rule >= num_rules) {
		print_solution(rules, num_rules, my_ticket);
		return;
	}

	for (int field = 0; field < num_rules; field++) {
		if (!field_used[field] && !rules[current_rule].impossible_fields[field]) {
			field_used[field] = 1;
			rules[current_rule].field = field;
			permute1(rules, current_rule + 1, num_rules, my_ticket);
			field_used[field] = 0;
		}
	}
}

void swap(int* a, int* b) {
	int temp = *a;
	*a = *b;
	*b = temp;
}

void permute2(struct Rule rules[], int current_rule, int num_rules, int my_ticket[]) {
	if (current_rule >= num_rules) {
		print_solution(rules, num_rules, my_ticket);
		return;
	}

	for (int rule = current_rule; rule < num_rules; rule++) {
		swap(&(rules[current_rule].field), &(rules[rule].field));
		if (!rules[current_rule].impossible_fields[rules[current_rule].field]) {
			permute2(rules, current_rule + 1, num_rules, my_ticket);
		}
		swap(&(rules[current_rule].field), &(rules[rule].field));
	}
}

void permute3(struct Rule rules[], int current_rule, int num_rules, int my_ticket[]) {
	if (current_rule >= num_rules) {
		print_solution(rules, num_rules, my_ticket);
		return;
	}

	for (int rule = current_rule; rule < num_rules; rule++) {
		swap(&(rules[current_rule].field), &(rules[rule].field));
		if (!rules[current_rule].impossible_fields[rules[current_rule].field]) {
			permute3(rules, current_rule + 1, num_rules, my_ticket);
		}
		SWAP(rules[current_rule].field, rules[rule].field);
	}
}

void solve1(struct Rule rules[], int num_rules, int my_ticket[]) {
	permute1(rules, 0, num_rules, my_ticket);
}

void solve2(struct Rule rules[], int num_rules, int my_ticket[]) {
	for (int i = 0; i < num_rules; i++) {
		rules[i].field = i;
	}
	permute2(rules, 0, num_rules, my_ticket);
}

void solve3(struct Rule rules[], int num_rules, int my_ticket[]) {
	for (int i = 0; i < num_rules; i++) {
		rules[i].field = i;
	}
	permute3(rules, 0, num_rules, my_ticket);
}

void call(char* name, void func(struct Rule*, int, int*), struct Rule arg1[], int arg2, int* arg3) {
	time_t start = time(NULL);
	func(arg1, arg2, arg3);
	time_t stop = time(NULL);
	printf("Function '%s' execution time: %ld seconds.\n\n", name, stop - start);
}

int main() {
	struct Rule rules[MAX_FIELDS];
	int num_rules = 0;
	FILE* fp = fopen("input.txt", "r");
	char buf[1000];
	while (1) {
		fgets(buf, 1000, fp);
		char* colon = strchr(buf, ':');
		if (colon == NULL) {
			break;
		}
		*colon = '\0';
		assert(num_rules < MAX_FIELDS);
		strcpy(rules[num_rules].name, buf);
		char* rule = colon + 2;
		char* rule2 = strstr(rule, " or ");
		*rule2 = '\0';
		rule2 += 4;
		extract_numbers(&(rules[num_rules].min1), &(rules[num_rules].max1), rule);
		extract_numbers(&(rules[num_rules].min2), &(rules[num_rules].max2), rule2);
		num_rules++;
	}

	fgets(buf, 1000, fp);
	assert(!strcmp(buf, "your ticket:\n"));
	fgets(buf, 1000, fp);
	int my_ticket[50];
	int num_fields = parse_ticket(my_ticket, buf);
	assert(num_fields == num_rules);

	fgets(buf, 1000, fp);
	fgets(buf, 1000, fp);
	assert(!strcmp(buf, "nearby tickets:\n"));
	int tickets[500][num_fields];
	int num_tickets = 0;
	while (1) {
		if(fgets(buf, 1000, fp) == NULL) {
			break;
		}
		assert(parse_ticket(tickets[num_tickets++], buf) == num_fields);
	}

	fclose(fp);

	int error_rate = 0;
	int ticket_invalid[num_tickets];
	memset(ticket_invalid, 0, sizeof(ticket_invalid));
	for (int ticket = 0; ticket < num_tickets; ticket++) {
		for (int field = 0; field < num_fields; field++) {
			int val = tickets[ticket][field];
			int invalid = 1;
			for (int rule = 0; rule < num_rules; rule++) {
				if (rule_fulfilled(rules + rule, val)) {
					invalid = 0;
					break;
				}
			}
			if (invalid) {
				error_rate += val;
				ticket_invalid[ticket] = 1;
			}
		}
	}

	printf("error rate: %d\n", error_rate);


	// Part 2

	for(int ticket = 0; ticket < num_tickets; ticket++) {
		if (ticket_invalid[ticket]) {
			continue;
		}
		for (int field = 0; field < num_fields; field++) {
			int val = tickets[ticket][field];
			for (int rule = 0; rule < num_rules; rule++) {
				if (!rule_fulfilled(rules + rule, val)) {
					rules[rule].impossible_fields[field] = 1;
				}
			}
		}
	}
	for (int rule = 0; rule < num_rules; rule++) {
		rules[rule].field = rule;
	}
	call("permute1", solve1, rules, num_rules, my_ticket);
	call("permute2", solve2, rules, num_rules, my_ticket);
	call("permute3", solve3, rules, num_rules, my_ticket);
}
