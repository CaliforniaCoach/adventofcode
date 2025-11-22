#include <stdio.h>
#include <assert.h>

long calc_loopsize(long pubkey, long subject_number) {
	long val = 1;
	long loopsize = 0;
	while (val != pubkey) {
		val *= subject_number;
		val %= 20201227;
		loopsize++;
	}
	return loopsize;
}

long transform(long loopsize, long subject_number) {
	long val = 1;
	for (long i = 0; i < loopsize; i++) {
		val *= subject_number;
		val %= 20201227;
	}
	return val;
}

int main() {
	FILE* fp = fopen("input.txt", "r");

	long pubkey1;
	fscanf(fp, "%ld", &pubkey1);
	long loopsize1 = calc_loopsize(pubkey1, 7);

	long pubkey2;
	fscanf(fp, "%ld", &pubkey2);
	long loopsize2 = calc_loopsize(pubkey2, 7);

	fclose(fp);

	long secretkey = transform(loopsize1, pubkey2);
	assert(secretkey == transform(loopsize2, pubkey1));

	printf("Secret key: %ld\n", secretkey);
}
