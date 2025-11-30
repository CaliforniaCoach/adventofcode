#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <limits.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

struct Result {
	int sum_versions;
	__uint128_t value;
};

unsigned long long get_bits(unsigned long long data[], int* idx, int* offset, int num_bits) {
	unsigned long long retval;
	if (num_bits <= *offset + 1) {
		retval = data[*idx] >> (*offset - num_bits + 1) & ((1ULL << num_bits) - 1);
		*offset -= num_bits;
		if (*offset < 0) {
			*offset += 64;
			(*idx)++;
		}
	} else {
		retval = data[*idx] & ((1ULL << (*offset + 1)) - 1);
		retval <<= num_bits - *offset - 1;
		(*idx)++;
		retval |= data[*idx] >> (64 - num_bits + *offset + 1);
		*offset = *offset - num_bits + 64;
	}
	return retval;
}

int real_modulo(int a, int b) {
	return a >= 0 ? a % b : b + a % b;
}

struct Result decode(unsigned long long data[], int* idx, int* offset) {
	struct Result result = {};
	result.sum_versions = (int) get_bits(data, idx, offset, 3);
	int type = (int) get_bits(data, idx, offset, 3);
	if (type == 4) {
		while (1) {
			int prefix = (int) get_bits(data, idx, offset, 1);
			assert(result.value <= ~((__uint128_t) 0) >> 4);
			result.value = result.value << 4 | (unsigned int) get_bits(data, idx, offset, 4);
			if (!prefix) {
				break;
			}
		}
	} else {
		struct Result sub_results[1 << 11];
		int num_sub_results = 0;
		int length_type = (int) get_bits(data, idx, offset, 1);
		if (length_type == 0) {
			int length = (int) get_bits(data, idx, offset, 15);
			int expected_idx = *idx + (length - *offset + 63) / 64;
			int expected_offset = real_modulo(*offset - length, 64);
			while (*idx < expected_idx || *offset > expected_offset) {
				sub_results[num_sub_results++] = decode(data, idx, offset);
			}
			assert(*idx == expected_idx && *offset == expected_offset);
		} else {
			int num_sub_packets = (int) get_bits(data, idx, offset, 11);
			for (int sub_packet = 0; sub_packet < num_sub_packets; sub_packet++) {
				sub_results[num_sub_results++] = decode(data, idx, offset);
			}
		}
		if (type >= 0 && type <= 3) {
			switch (type) {
				case 0: result.value = 0; break;
				case 1: result.value = 1; break;
				case 2: result.value = ~((__uint128_t) 0); break;
				case 3: result.value = (__uint128_t) 0; break;
			}
			for (int i = 0; i < num_sub_results; i++) {
				result.sum_versions += sub_results[i].sum_versions;
				__uint128_t subval = sub_results[i].value;
				switch (type) {
					case 0:
						assert(result.value <= ~((__uint128_t) 0) - subval);
						result.value += subval;
						break;
					case 1:
						assert(result.value <= ~((__uint128_t) 0) / subval);
						result.value *= subval;
						break;
					case 2:
						result.value = MIN(result.value, subval);
						break;
					case 3:
						result.value = MAX(result.value, subval);
						break;
				}
			}
		} else if (type >= 5 && type <= 7) {
			assert(num_sub_results == 2);
			result.sum_versions += sub_results[0].sum_versions + sub_results[1].sum_versions;
			switch (type) {
				case 5: result.value = sub_results[0].value > sub_results[1].value ? 1 : 0; break;
				case 6: result.value = sub_results[0].value < sub_results[1].value ? 1 : 0; break;
				case 7: result.value = sub_results[0].value == sub_results[1].value ? 1 : 0; break;
			}
		} else {
			assert(0);
		}
	}
	return result;
}

void process(char* hex_string) {
	unsigned long long data[1000];
	int data_size = 0;
	while (16 * data_size < (int) strlen(hex_string)) {
		char digits[17] = {0};
		strncpy(digits, hex_string + 16 * data_size, 16);
		while (strlen(digits) < 16) {
			digits[strlen(digits)] = '0';
		}
		data[data_size++] = strtoull(digits, NULL, 16);
	}

	int idx = 0;
	int offset = 63;
	struct Result result = decode(data, &idx, &offset);
	assert((data[idx] & ((1ULL << (offset + 1)) - 1)) == 0);
	for (int i = idx + 1; i < data_size; i++) {
		assert(data[i] == 0ULL);
	}
	assert(result.value <= ULLONG_MAX);
	printf("Sum versions: %d, value: %lld\n", result.sum_versions,
		(unsigned long long) result.value
	);
}

void test(char* hex_string, int expected_value) {
	printf("Expecting value %d:\n", expected_value);
	process(hex_string);
	puts("");
}

int main() {
	char string[10000];
	FILE* fp = fopen("input.txt", "r");
	fgets(string, 10000, fp);
	fclose(fp);
	string[strlen(string) - 1] = '\0';

	printf("Tests:\n\n");
	test("C200B40A82", 3);
	test("04005AC33890", 54);
	test("880086C3E88112", 7);
	test("CE00C43D881120", 9);
	test("D8005AC2A8F0", 1);
	test("F600BC2D8F", 0);
	test("9C005AC2F8F0", 0);
	test("9C0141080250320F1802104A08", 1);

	printf("\nFinal result:\n\n");
	process(string);
}
