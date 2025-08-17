#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum Data {BYR, IYR, EYR, HGT, HCL, ECL, PID, CID};

enum Data str2enum(char string[]) {
	if (!strcmp(string, "byr")) return BYR;
	if (!strcmp(string, "iyr")) return IYR;
	if (!strcmp(string, "eyr")) return EYR;
	if (!strcmp(string, "hgt")) return HGT;
	if (!strcmp(string, "hcl")) return HCL;
	if (!strcmp(string, "ecl")) return ECL;
	if (!strcmp(string, "pid")) return PID;
	if (!strcmp(string, "cid")) return CID;
	return -1;
}

int is_valid(int does_contain[]) {
	for (enum Data d = 0; d < 8; d++) {
		if (d != CID && !does_contain[d]) {
			return 0;
		}
	}
	return 1;
}

int is_value_valid(enum Data key, char value[]) {
	int iVal = atoi(value);
	switch (key) {
		case BYR:
			return strlen(value) == 4 && iVal >= 1920 && iVal <= 2002;
		case IYR:
			return strlen(value) == 4 && iVal >= 2010 && iVal <= 2020;
		case EYR:
			return strlen(value) == 4 && iVal >= 2020 && iVal <= 2030;
		case HGT:
			char* unit = value;
			while (*unit >= '0' && *unit <= '9') {
				unit++;
			}
			if (!strcmp(unit, "cm")) {
				return iVal >= 150 && iVal <= 193;
			} else if (!strcmp(unit, "in")) {
				return iVal >= 59 && iVal <= 76;
			} else {
				return 0;
			}
		case HCL:
			if (strlen(value) != 7 || value[0] != '#') {
				return 0;
			}
			for (int i = 0; i < 6; i++) {
				char val = value[i + 1];
				if (!((val >= '0' && val <= '9') || (val >= 'a' && val <= 'f'))) {
					return 0;
				}
			}
			return 1;
		case ECL:
			return !(strcmp(value, "amb") && strcmp(value, "blu") && strcmp(value, "brn") &&
			         strcmp(value, "gry") && strcmp(value, "grn") && strcmp(value, "hzl") &&
			         strcmp(value, "oth"));
		case PID:
			if (strlen(value) != 9) {
				return 0;
			}
			for (int i = 0; i < 9; i++) {
				if (value[i] < '0' || value[i] > '9') {
					return 0;
				}
			}
			return 1;
		case CID:
			return 1;
	}
}

int main() {
	char buf[1000];
	int does_contain[8] = {0};
	int valid_value[8] = {0};
	int count1 = 0, count2 = 0;
	FILE* fp = fopen("input.txt", "r");
	while (fgets(buf, 1000, fp)) {
		buf[strlen(buf) - 1] = '\0';
		if (strlen(buf) == 0) {
			count1 += is_valid(does_contain);
			count2 += is_valid(valid_value);
			memset(does_contain, 0, sizeof(valid_value) * sizeof(int));
			memset(valid_value, 0, sizeof(valid_value) * sizeof(int));
		} else {
			char* tok = strtok(buf, " ");
			while (tok) {
				char key[10] = {0};
				char* separator = strchr(tok, ':');
				strncpy(key, tok, separator - tok);
				enum Data key_enum = str2enum(key);
				does_contain[key_enum] = 1;
				valid_value[key_enum] = is_value_valid(key_enum, separator + 1);
				tok = strtok(NULL, " ");
			}
		}
	}
	fclose(fp);
	count1 += is_valid(does_contain);
	count2 += is_valid(valid_value);

	printf("%d\n", count1);
	printf("%d\n", count2);
}
