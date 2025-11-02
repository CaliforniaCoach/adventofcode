#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>

typedef unsigned long ulong;

#define BUF2MAXSIZE 100000

ulong addr1[1000];
ulong val1[1000];
ulong addr2[BUF2MAXSIZE];
ulong val2[BUF2MAXSIZE];
int bufsize1 = 0;
int bufsize2 = 0;

int get_index(ulong addr_buf[], int bufsize, ulong address) {
	for (int i = 0; i < bufsize; i++) {
		if (addr_buf[i] == address) {
			return i;
		}
	}
	return -1;
}

void set(ulong addr_buf[], ulong val_buf[], int* bufsize, ulong address, ulong value) {
	int i = get_index(addr_buf, *bufsize, address);
	if (i >= 0) {
		val_buf[i] = value;
	} else {
		assert(*bufsize < BUF2MAXSIZE);
		addr_buf[*bufsize] = address;
		val_buf[(*bufsize)++] = value;
	}
}

ulong get_sum(ulong val_buf[], int bufsize) {
	ulong sum = 0;
	for (int i = 0; i < bufsize; i++) {
		assert(ULONG_MAX - sum >= val_buf[i]);
		sum += val_buf[i];
	}
	return sum;
}

void apply_mask(ulong addr, ulong value, ulong mask_x, ulong shift) {
	if (!mask_x) {
		set(addr2, val2, &bufsize2, addr, value);
	} else {
		while (!(mask_x & 1)) {
			mask_x >>= 1; 
			shift++;
		}
		apply_mask(addr & ~(1UL << shift), value, mask_x >> 1, shift + 1);
		apply_mask(addr | (1UL << shift), value, mask_x >> 1, shift + 1);
	}
}

int main() {
	char buf[500];
	ulong mask_zero, mask_one, mask_x;
	FILE* fp = fopen("input.txt", "r");
	while (fgets(buf, 500, fp)) {
		char mask[100];
		ulong address;
		long value;
		if (sscanf(buf, " mask = %s ", mask) >= 1) {
			mask_zero = 0UL;
			mask_one = 0UL;
			mask_x = 0UL;
			char* ptr = mask;
			while (*ptr != '\0') {
				mask_zero <<= 1;
				mask_one <<= 1;
				mask_x <<= 1;
				if (*ptr == '0') {
					mask_zero |= 1;
				} else if (*ptr == '1') {
					mask_one |= 1;
				} else if (*ptr == 'X') {
					mask_x |= 1;
				} else {
					exit(1);
				}
				ptr++;
			}
		} else if (sscanf(buf, " mem[%ld] = %ld ", &address, &value) >= 1) {
			assert(value >= 0);
			set(addr1, val1, &bufsize1, address, ((ulong) value & ~mask_zero) | mask_one);
			apply_mask(address | mask_one, (ulong) value, mask_x, 0);
		} else {
			exit(1);
		}
	}
	printf("%ld\n", get_sum(val1, bufsize1));
	printf("%ld\n", get_sum(val2, bufsize2));
}

// 2902947162769 too low
// 2150982919749