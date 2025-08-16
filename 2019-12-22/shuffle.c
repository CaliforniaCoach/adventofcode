#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define FILENAME "input.txt"

#define DECKSIZE1 10007
#define START_POS1 2019

#define DECKSIZE2 119315717514047
#define PROCESSES 101741582076661
#define TARGET_POS2 2020

#define CUT 1
#define DEAL 2
#define REVERSE 3

long gcdExtendedRecursive(long a, long b, long* s, long* t)
{
    if (b == 0)
    {
        *s = 1;
        *t = 0;
        return a;
    }
    long s1, t1;
    long d = gcdExtendedRecursive(b, a % b, &s1, &t1);
    *s = t1;
    *t = s1 - (a / b) * t1;
    return d;
}

long mod(__int128 a, long b) {
	long c = a % b;
	return c < 0 ? c + b : c;
}

__int128 pow_mod(long base, long exp, __int128 mod)
{
    __int128 result = 1L;
	__int128 big_base = base;
    while (1) {
        if (exp & 1L)
            result = (_BitInt(256)) result * big_base % mod;
        exp >>= 1;
        if (!exp)
            break;
        big_base =  (_BitInt(256)) big_base * big_base % mod;
    }
    return result;
}

int main() {
	int deck[DECKSIZE1];
	for (int i = 0; i < sizeof(deck) / sizeof(int); i++) {
		deck[i] = i;
	}

	int commands[200];
	int parameter[200];
	char buf[100];
	FILE* fp = fopen(FILENAME, "r");
	int param;
	int i;
	for (i = 0; i < 200; i++) {
		if (!fgets(buf, 100, fp)) {
			break;
		}
		if (sscanf(buf, "cut %d\n", &param)) {
			commands[i] = CUT;
			parameter[i] = param;
		} else if (sscanf(buf, "deal with increment %d\n", &param)) {
			commands[i] = DEAL;
			parameter[i] = param;
		} else if (!strcmp(buf, "deal into new stack\n")) {
			commands[i] = REVERSE;
		} else {
			printf("Unknown command found: '%s'\n", buf);
			exit(1);
		}
	}

	int num_commands = i;
	printf("Read %d commands.\n", num_commands);

	int helpdeck[DECKSIZE1];
	for (int k = 0; k < num_commands; k++) {
		int param = parameter[k];
		switch (commands[k]) {
		case REVERSE:
			for (int j = 0; j < DECKSIZE1 / 2; j++) {
				int swap = deck[j];
				deck[j] = deck[DECKSIZE1 - 1 - j];
				deck[DECKSIZE1 - 1 - j] = swap;
			}
			break;

		case CUT:
			if (param < 0) {
				param = DECKSIZE1 - (-param);
			}
			memcpy(helpdeck, deck, param * sizeof(int));
			memcpy(deck, deck + param, (DECKSIZE1 - param) * sizeof(int));
			memcpy(deck + (DECKSIZE1 - param), helpdeck, param * sizeof(int));
			break;

		case DEAL:
			for (int j = 0; j < DECKSIZE1; j++) {
				helpdeck[(j * param) % DECKSIZE1] = deck[j];
			}
			memcpy(deck, helpdeck, DECKSIZE1 * sizeof(int));
			break;

		default:
			printf("Unexpected command at index %d\n", k);
			exit(1);
		}
	}
	if (DECKSIZE1 > START_POS1) {
		for (int i = 0; i < DECKSIZE1; i++) {
			if (deck[i] == START_POS1) {
				printf("Card %d is at position: %d\n", START_POS1, i);
			}
		}
	} else {
		for (int i = 0; i < DECKSIZE1; i++) {
			printf("%d ", deck[i]);
		}
		puts("");
	}

	// Part 2

	// #define DECKSIZE2 DECKSIZE
	// #define PROCESSES 1
	// #define TARGET_POS2 7614

	// #define DECKSIZE2 DECKSIZE
	// #define PROCESSES 3
	// #define TARGET_POS2 1994

	// Pre-calculate Euclid parameters
	long s[100], t[100], g[100] = {0};
	for (int command = 0; command < num_commands; command++) {
		int param = parameter[command];
		if (commands[command] == DEAL && g[param] == 0) {
			assert(param < 100);
			g[param] = gcdExtendedRecursive(param, DECKSIZE2, s + param, t + param);
			assert(g[param] == 1);
		}
	}

	// pos[i] = mul[i] * pos[i - 1] + add[i]
	// pos[i - 1] = mul[i - 1] * pos[i - 2] + add[i - 1]
	// => pos[i] = mul[i] * (mul[i - 1] * pos[i - 2] + add[i - 1]) + add[i]
	//           = (mul[i] * mul[i - 1]) * pos[i - 2] + (mul[i] * add[i - 1] + add[i])
	__int128 mul = 1L;
	__int128 add = 0L;
	long mul_next;
	long add_next;
	for (long process = 0; process < 1; process++) {
		for (int k = num_commands - 1; k >= 0; k--) {
			int param = parameter[k];
			switch (commands[k]) {
				case REVERSE:
					// from_pos = -from_pos - 1LL;
					mul_next = -1;
					add_next = -1;
					break;

				case CUT:
					// from_pos = from_pos + param;
					mul_next = 1;
					add_next = param;
					break;

				case DEAL:
					// from_pos = s[param] * from_pos / g[param];
					mul_next = s[param];
					add_next = 0;
					break;
			}
			mul = mul * mul_next % DECKSIZE2;
			add = (mul_next * add + add_next) % DECKSIZE2;
		}
	}
	// pos[i] = mul * pos[i - 1] + add
	//        = mul * (mul * pos[i - 2] + add) + add
	//        = (mul * mul) * pos[i - 2] + mul * add + add
	//        = (mul * mul) * (mul * pos[i - 3] + add) + mul * add + add
	//        = (mul * mul * mul) pos[i - 3] + mul * mul * add + mul * add + add
	//        = pow(mul, n) * pos[i - n] + (pow(mul, 0) + pow(mul, 1) + pow(mul, 2) + ... + pow(mul, n - 1)) * add
	//        = pow(mul, n) * pos[i - n] + ((pow(mul, n) - 1) / (mul - 1)) * add

	// (a / b) mod m = (a mod (m * b)) / b
	//                       !!! ^^^ !!!
	// e.g. (12 / 2) mod 4 = 2
	// = ((12 mod (2 * 4)) / 2) = 2
	// != (12 mod 4) / 2 = 0

	// e.g. (18 / 3) mod 4 = 2
	// = ((18 mod (3 * 4)) / 3) = 2
	// != ((18 mod 4) / 3 = 2/3
	long n = PROCESSES;
	__int128 from_pos = pow_mod(mul, n, DECKSIZE2) * TARGET_POS2 + ((pow_mod(mul, n, DECKSIZE2 * (mul - 1)) - 1) / (mul - 1)) * add;
	from_pos = mod(from_pos, DECKSIZE2);
	printf("At position %d , we have card: %ld\n", TARGET_POS2, from_pos);
}
