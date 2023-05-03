
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

/* [0, 1) */
#define RANDOM()    (random() / (RAND_MAX + 1.0))
#define ARRAY_MAX   256

struct union_find {
	int data_size;
	int father[ARRAY_MAX];
	int size[ARRAY_MAX];
	int help[ARRAY_MAX];
};

int find_father(struct union_find *u_find, int x)
{
	int h_idx = 0;
	while (x != u_find->father[x]) {
		u_find->help[h_idx++] = x;
		x = u_find->father[x];
	}

	for (h_idx--; h_idx >= 0; h_idx--) {
		u_find->father[u_find->help[h_idx]] = x;
	}

	return x;
}

bool is_same_set(struct union_find *u_find, int x, int y)
{
	if (x >= u_find->data_size || y >= u_find->data_size) {
		return false;
	}

	return find_father(u_find, x) == find_father(u_find, y);
}

void union_f(struct union_find *u_find, int x, int y)
{
	if (x >= u_find->data_size || y >= u_find->data_size) {
		return;
	}

	int x_f = find_father(u_find, x);
	int y_f = find_father(u_find, y);

	if (x_f != y_f) {
		if (u_find->size[x_f] >= u_find->size[y_f]) {
			u_find->size[x_f] += u_find->size[y_f];
			u_find->father[y_f] = x_f;
		} else {
			u_find->size[y_f] += u_find->size[x_f];
			u_find->father[x_f] = y_f;
		}
	}
}

int union_init(struct union_find *u_find, int n)
{
	u_find->data_size = n;

	for (int i = 0; i < n; i++) {
		u_find->father[i] = i;
		u_find->size[i] = 1;
	}

	return 0;
}

int main(int argc, char **argv)
{
	int max_op = 20;
	int max_value = 10;
	int times = 10;

	srandom((unsigned) time(0));

	for (int i = 0; i < times; i++) {
		struct union_find u_find = {
			.data_size = 0,
			.father = { -1, },
			.size = { -1, },
			.help = { -1, },
		};

		int n = (int)(RANDOM() * max_value);
		printf("n = %d\n", n);
		union_init(&u_find, n);

		int m = (int)(RANDOM() * max_op);
		for (int j = 0; j < m; j++) {
			int x = (int)(RANDOM() * max_value);
			int y = (int)(RANDOM() * max_value);
			if (x == y) {
				y += 1;
			}

			if (RANDOM() < 0.5) {
				printf("(%2d, %2d) is same set ? .. %d\n", x, y, is_same_set(&u_find, x, y));
			} else {
				printf("union_f (%2d, %2d)\n", x, y);
				union_f(&u_find, x, y);
			}
		}

		printf("father:\t");
		for (int k = 0; k <= n; k++) {
			printf("(%2d, %2d), ", k, u_find.father[k]);
		}
		printf("\n");

		printf("---8<---------------------------------------------------\n\n");
	}

	return 0;
}
