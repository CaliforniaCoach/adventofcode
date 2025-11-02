#include <stdio.h>
#include <limits.h>
#include <assert.h>

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

int main() {
	int min_dep;
	int ids[1000];
	int num_ids = 0;
	FILE* fp = fopen("input.txt", "r");
	fscanf(fp, " %d ", &min_dep);
	while (!feof(fp)) {
		if (fscanf(fp, "%d", ids + num_ids) == 0) {
			fscanf(fp, "x");
			ids[num_ids] = -1;
		}
		num_ids++;
		fscanf(fp, " , ");
	}
	fclose(fp);

	int min_wait_time = INT_MAX, min_id;
	for (int i = 0; i < num_ids; i++) {
		if (ids[i] > 0) {
			int dep_time = min_dep - min_dep % ids[i];
			int wait_time = dep_time >= min_dep ? dep_time - min_dep : dep_time - min_dep + ids[i];
			if (wait_time < min_wait_time) {
				min_wait_time = wait_time;
				min_id = ids[i];
			}
		}
	}
	printf("%d\n", min_wait_time * min_id);

	// Part 2
	// Searched point in time: t
	// t mod <bus1 ID> = <bus1 ID> - <bus1 pos in list>
	// t mod <bus2 ID> = <bus2 ID> - <bus2 pos in list>
	// ...
	// or:
	// x mod n1 = a1
	// x mod n2 = a2
	// ...
	// Solve with Chinese remainder theorem
	long N = 1;
	long x = 0;
	for (int i = 0; i < num_ids; i++) {
		if (ids[i] > 0) {
			long old_N = N;
			N *= ids[i];
			assert(N / ids[i] == old_N);
		}
	}
	for (int i = 0; i < num_ids; i++) {
		if (ids[i] > 0) {
			long si, ti;
			long ni = ids[i];
			long ai = (ni - i % ni) % ni;
			assert(ai >= 0);
			long Ni = N / ni;
			long gcd = gcdExtendedRecursive(ni, Ni, &si, &ti);
			assert(gcd == 1);
			long ei = ti * Ni;
			x += ai * ei;
		}
	}
	x = (x % N + N) % N;
	printf("%ld\n", x);
}
