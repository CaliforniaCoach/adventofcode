#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


enum Operator {PLUS, TIMES};


char* eval(char* line, long* result) {
	long operand = 0;
	enum Operator operator = PLUS;
	while (1) {
		if (*line == ' ') {
			// do nothing
		} else if (*line == '(') {
			line = eval(line + 1, &operand);
			*result = operator == PLUS ? *result + operand : *result * operand;
			operand = 0;
		} else if (*line == ')' || *line == '\n' || *line == '\0') {
			return line;
		} else if (*line == '+') {
			operator = PLUS;
		} else if (*line == '*') {
			operator = TIMES;
		} else if (*line >= '0' && *line <= '9') {
			while (*line >= '0' && *line <= '9') {
				operand = operand * 10 + *(line++) - '0';
			}
			*result = operator == PLUS ? *result + operand : *result * operand;
			operand = 0;
			line--;
		} else {
			assert(0);
		}
		line++;
	}
}

char* eval2(char* line, long* result) {
	*result = 1;
	long sum = 0;
	while (1) {
		if (*line == ' ') {
			// do nothing
		} else if (*line == '(') {
			long operand;
			line = eval2(line + 1, &operand);
			sum += operand;
		} else if (*line == ')' || *line == '\n' || *line == '\0') {
			*result *= sum;
			return line;
		} else if (*line == '+') {
			// do nothing
		} else if (*line == '*') {
			*result *= sum;
			sum = 0;
		} else if (*line >= '0' && *line <= '9') {
			long operand = *line - '0';
			sum += operand;
		} else {
			assert(0);
		}
		line++;
	}
}

int main() {

	long result;

	result = 0;
	eval("2 * 3 + (4 * 5)", &result);
	printf("Expecting 26: %ld\n", result);

	result = 0;
	eval("5 + (8 * 3 + 9 + 3 * 4 * 3)", &result);
	printf("Expecting 437: %ld\n", result);

	result = 0;
	eval("5 * 9 * (7 * 3 * 3 + 9 * 3 + (8 + 6 * 4))", &result);
	printf("Expecting 12240: %ld\n", result);

	result = 0;
	eval("((2 + 4 * 9) * (6 + 9 * 8 + 6) + 6) + 2 + 4 * 2", &result);
	printf("Expecting 13632: %ld\n", result);

	puts("");

	eval2("1 + (2 * 3) + (4 * (5 + 6))", &result);
	printf("Expecting 51: %ld\n", result);

	eval2("((2 + 4 * 9) * (6 + 9 * 8 + 6) + 6) + 2 + 4 * 2", &result);
	printf("Expecting 23340: %ld\n", result);

	puts("");
	long sum1 = 0;
	long sum2 = 0;
	FILE* fp = fopen("input.txt", "r");
	char line[500];
	while (fgets(line, 500, fp)) {
		long result = 0;
		eval(line, &result);
		sum1 += result;
		eval2(line, &result);
		sum2 += result;
	}
	fclose(fp);

	printf("Sum part 1: %ld\n", sum1);
	printf("Sum part 2: %ld\n", sum2);
}