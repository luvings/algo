
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* [0, 1) */
#define RANDOM()        (random() / (RAND_MAX + 1.0))

int os_get_random(unsigned char *buf, size_t len)
{
	FILE *f;
	size_t rc;

	f = fopen("/dev/urandom", "rb");
	if (f == NULL) {
		printf("could not open /dev/urandom.\n");
		return -1;
	}

	rc = fread(buf, 1, len, f);
	fclose(f);

	return rc != len ? -1 : 0;
}

int main(int argc, char *argv[])
{
	int size = 5;
	unsigned int seed;

	//srandom((unsigned) time(0));

	if (os_get_random((unsigned char *)&seed, sizeof(seed)) == 0) {
		srandom(seed);
	}
	printf("seed = %u\n", seed);

	for (int i = 0; i < size; i++) {
		printf("%4d, ", (int)(10 * RANDOM()));
	}
	printf("\n");

	return 0;
}
