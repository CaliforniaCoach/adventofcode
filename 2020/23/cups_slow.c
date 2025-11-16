#include <stdio.h>
#include <string.h>
#include <inttypes.h>


#define INPUT "716892543"
// #define INPUT "389125467"


void move(uint32_t list[], uint32_t src_idx, uint32_t dest_idx, uint32_t size) {
	/*
	uint32_t copy_size = src_idx - dest_idx - 3 + 1;
	if (src_idx <= size - 3) {
		if (dest_idx <= size - 3) {
			memmove(list + dest_idx, list + src_idx, copy_size * sizeof(uint32_t));
		}
	}
	memcpy(list + dest_idx, list + src_idx, sizeof(uint32_t));
	*/
	uint32_t temp1 = list[(dest_idx + 0) % size];
	uint32_t temp2 = list[(dest_idx + 1) % size];
	uint32_t temp3 = list[(dest_idx + 2) % size];

	/*
	uint32_t* dest_ptr = list + dest_idx;
	uint32_t* src_ptr = list + (dest_idx + 3) % size;
	uint32_t* end_buf = list + size;
	uint32_t* end_dest_ptr = list + (src_idx + size + 1 - 3) % size;
	while (dest_ptr != end_dest_ptr) {
		*dest_ptr = *src_ptr;
		if (++dest_ptr >= end_buf) {
			dest_ptr = list;
		}
		if (++src_ptr >= end_buf) {
			src_ptr = list;
		}
	}
	*/
	uint32_t to_k = (src_idx - dest_idx - 3 + size) % size;
	for (uint32_t k = 0; k <= to_k; k++) {
		list[(dest_idx + k) % size] = list[(dest_idx + k + 3) % size];
	}
	list[(src_idx - 2 + size) % size] = temp1;
	list[(src_idx - 1 + size) % size] = temp2;
	list[(src_idx - 0 + size) % size] = temp3;
}

uint32_t solve(uint32_t cups[], uint32_t size, uint32_t moves) {
	for (uint32_t i = 0; i < size; i++) {
		cups[i] = i < strlen(INPUT) ? (uint32_t) (INPUT[i] - '0') : i;
	}
	uint32_t current_idx = 0;

	for (uint32_t i = 0; i < moves; i++) {
		uint32_t current = cups[current_idx];
		uint32_t destination = current;
		uint32_t destination_idx = UINT32_MAX;
		while (destination_idx == UINT32_MAX) {
			if (--destination == 0) {
				destination = size;
			}
			for (uint32_t k = 4; k < size; k++) {
				if (cups[(current_idx + k) % size] == destination) {
					destination_idx = (current_idx + k) % size;
					break;
				}
			}
		}
		move(cups, destination_idx, (current_idx + 1) % size, size);
		current_idx = (current_idx + 1) % size;
		if (i && !(i % 1000)) {
			printf("Move %d\n", i);
		}
	}
	uint32_t idx_one = 0;
	while (cups[idx_one] != 1) {
		idx_one++;
	}
	return idx_one;
}

int main() {
	uint32_t cups[1000000];

	uint32_t size = strlen(INPUT);
	uint32_t idx_one = solve(cups, size, 100);
	for (uint32_t i = 1; i < size; i++) {
		printf("%d", cups[(idx_one + i) % size]);
	}
	puts("");

	size = 1000000;
	idx_one = solve(cups, size, 10000000);
	printf("%"PRIu64"\n", (uint64_t) cups[(idx_one + 1) % size] * cups[(idx_one + 2) % size]);
}
