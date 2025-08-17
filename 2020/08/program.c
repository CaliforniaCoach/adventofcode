#include <stdio.h>
#include <string.h>
#include <stdlib.h>

enum Op {ACC, JMP, NOP} prg[1000];
int param[1000];
int prglen = 0;

int run(int* accu) {
	int executed[1000] = {0};
	int pc = 0;
	while (pc >= 0 && pc < prglen && !executed[pc]) {
		executed[pc] = 1;
		switch (prg[pc]) {
			case ACC:
				*accu += param[pc++];
				break;
			case JMP:
				pc += param[pc];
				break;
			case NOP:
				pc++;
				break;
			default:
				exit(2);
		}
	}
	return pc >= prglen;
}

int main() {
	char op[10];
	FILE* fp = fopen("input.txt", "r");
	while (!feof(fp)) {
		if (fscanf(fp, "%s %d ", op, param + prglen) != 2) {
			exit(1);
		}
		if (!strcmp(op, "acc")) {
			prg[prglen] = ACC;
		} else if (!strcmp(op, "jmp")) {
			prg[prglen] = JMP;
		} else if (!strcmp(op, "nop")) {
			prg[prglen] = NOP;
		} else {
			exit(1);
		}
		prglen++;
	}
	fclose(fp);

	int accu = 0;
	run(&accu);
	printf("ACCU: %d\n", accu);

	for (int i = 0; i < prglen; i++) {
		if (prg[i] == JMP || prg[i] == NOP) {
			prg[i] = prg[i] == JMP ? NOP : JMP;
			int accu = 0;
			if (run(&accu)) {
				printf("Terminated regularly with ACCU: %d\n", accu);
			}
			prg[i] = prg[i] == JMP ? NOP : JMP;
		}
	}
}
