
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <map>
#include <set>

/* [0, 1) */
#define RANDOM() (random() / (RAND_MAX + 1.0))

/*
 * 输入一定能够保证, 数组中所有的数都出现了 M 次, 只有一种数出现了 K 次
 * 1 <= K < M
 * 返回这种数
 */
int km(int arr[], int n, int k, int m)
{
	int help[32] = { 0, };

	for (int i = 0; i < n; i++) {
		int num = arr[i];

		for (int j = 0; j < 32; j++) {
			help[j] += (num >> j) & 1;
		}
	}

	int ans = 0;

	for (int i = 0; i < 32; i++) {
		help[i] %= m;
		if (help[i] != 0) {
			ans |= 1 << i;
		}
	}

	return ans;
}

int km_1(int arr[], int n, int k, int m)
{
	std::map < int, int > map;

	for (int i = 0; i < n; i++) {
		int num = arr[i];

		if (map.count(num)) {
			map[num] = map.find(num)->second + 1;
		} else {
			map.insert(std::make_pair(num, 1));
		}
	}

	int res = 0;

    for (auto it : map) {
		if (it.second == k) {
			res = it.first;
			break;
		}
	}

	return res;
}

int km_2(int arr[], int n, int k, int m)
{
	std::map < int, int > map;

	int val = 1;
	for (int i = 0; i < 32; i++) {
		map.insert(std::make_pair(val, i));
		val <<= 1;
	}

	/* t[0] 0 位置的 1 出现了几个 */
	/* t[i] i 位置的 1 出现了几个 */
	int t[32] = { 0, };

	for (int i = 0; i < n; i++) {
		int num = arr[i];

		while (num != 0) {
			int right_1 = num & (~num + 1);
			t[map.find(right_1)->second]++;
			num ^= right_1;
		}
	}

	int res = 0;

	/*
	 * 如果这个出现了 K 次的数, 是0
	 * 那么下面代码中的: ans |= (1 << i), 就不会发生
	 * 那么 ans 就会一直维持 0, 最后返回 0, 也是对的
	 */
	for (int i = 0; i < 32; i++) {
		if (t[i] % m != 0) {
			res |= (1 << i);
		}
	}

	return res;
}

int create_random_array_km(int max_size, int max_value, int k, int m, int *array, int *actual_size)
{
	/* [-9, +9] */
	int knum = (int)((max_value + 1) * RANDOM());

	/* 1 <= K < M */
	int kinds = (int)(RANDOM() * max_size) + 2;

	*actual_size = k * 1 + (kinds - 1) * m;

	int index = 0;
	for (; index < k; index++) {
		array[index] = knum;
	}

	kinds--;
	std::set < int > sets;
	sets.insert(knum);

	while (kinds != 0) {
		int num = 0;
		do {
			num = (int)((max_value + 1) * RANDOM());
		} while (sets.count(num));

		sets.insert(num);
		kinds--;

		for (int i = 0; i < m; i++) {
			array[index++] = num;
		}
	}

	for (int i = 0; i < *actual_size; i++) {
		int j = (int)(RANDOM() * (*actual_size));	/* 0 ~ N-1 */
		int tmp = array[i];
		array[i] = array[j];
		array[j] = tmp;
	}

	return 0;
}

void print_array_data(int *data, int n)
{
	if (data == NULL) {
		return;
	}

	for (int i = 0; i < n; i++) {
		printf("%4d\t", data[i]);
		if ((i + 1) % 16 == 0)
			printf("\n");
	}

	printf("\n");
}

int main(int argc, char *argv[])
{
	int max_size = 3;
	int max_value = 20;
	int max = 5;
	int times = 1000;
	bool succeed = true;

	srandom((unsigned) time(0));

	for (int i = 0; i < times; i++) {
		int arr1[100];
		int actual_size = 0;
		memset(arr1, 0, sizeof(arr1));

		/* a: 1 ~ 9 */
		int a = (int)(RANDOM() * max) + 1;
		int b = (int)(RANDOM() * max) + 1;
		int k = std::min(a, b);
		int m = std::max(a, b);
		/* k < m */
		if (k == m) {
			m++;
		}

		create_random_array_km(max_size, max_value, k, m, arr1, &actual_size);

		int r1 = km_1(arr1, actual_size, k, m);
		int r2 = km_2(arr1, actual_size, k, m);
		int r3 = km(arr1, actual_size, k, m);

		printf("[%d]:\t", actual_size);
		print_array_data(arr1, actual_size);
		printf("k = %d, m = %d, km = %d\n", k, m, r3);
		printf("---8<---------------------------------------------------\n\n");

		if (r1 != r2 || r1 != r3) {
			succeed = false;
			break;
		}
	}

	printf("result: %s\n", succeed ? "nice" : "fucked");

	return 0;
}
