#include <stdio.h>
#include <string.h>
#include <inttypes.h>


#define INPUT "716892543"


struct Cup {
	uint32_t label;
	struct Cup* next;
};


void move(struct Cup* current, struct Cup* dest) {
	struct Cup* triple = current->next;
	current->next = current->next->next->next->next;
	triple->next->next->next = dest->next;
	dest->next = triple;
}

struct Cup* solve(struct Cup cups[], uint32_t size, uint32_t moves) {
	struct Cup* current = cups + (INPUT[0] - '1');
	
	for (uint32_t i = 0; i < size; i++) {
		cups[i].label = i + 1;
		cups[i].next = i < size - 1 ? cups + i + 1 : current;
	}
	for (uint32_t i = 0; i < strlen(INPUT) - 1; i++) {
		cups[INPUT[i] - '1'].next = cups + (INPUT[i + 1] - '1');
	}
	if (size > strlen(INPUT)) {
		cups[INPUT[strlen(INPUT) - 1] - '1'].next = cups + strlen(INPUT);
	} else {
		cups[INPUT[strlen(INPUT) - 1] - '1'].next = current;
	}

	for (uint32_t i = 0; i < moves; i++) {
		uint32_t triple1 = current->next->label;
		uint32_t triple2 = current->next->next->label;
		uint32_t triple3 = current->next->next->next->label;
		uint32_t destination = current->label;
		do {
			if (--destination == 0) {
				destination = size;
			}
		} while (destination == triple1 || destination == triple2 || destination == triple3);
		move(current, cups + destination - 1);
		current = current->next;
	}
	struct Cup* one = cups;
	while (one->label != 1) {
		one = one->next;
	}
	return one;
}

int main() {
	static struct Cup cups[1000000];

	uint32_t size = strlen(INPUT);
	struct Cup* one = solve(cups, size, 100);
	for (struct Cup* cup = one->next; cup != one; cup = cup->next) {
		printf("%d", cup->label);
	}
	puts("");

	size = 1000000;
	one = solve(cups, size, 10000000);
	printf("%"PRIu64"\n", (uint64_t) one->next->label * one->next->next->label);
}
