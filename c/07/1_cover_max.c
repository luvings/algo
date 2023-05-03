
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <algorithm>
#include <queue>
#include <vector>

/* [0, 1) */
#define RANDOM()    (random() / (RAND_MAX + 1.0))

/*
 * 求最多重合线段的个数
 */
int cover_max_1(int lines[][2], int rows)
{
	int min = INT_MAX, max = INT_MIN;
	int cover = 0;

	for (int i = 0; i < rows; i++) {
		min = std::min(min, lines[i][0]);
		max = std::max(max, lines[i][1]);
	}

	for (double p = min + 0.5; p < max; p += 1) {
		int cnt = 0;
		for (int i = 0; i < rows; i++) {
			if (lines[i][0] < p && lines[i][1] > p) {
				cnt++;
			}
		}

		cover = std::max(cover, cnt);
	}

	return cover;
}

/*
 * m 是二维数组, 可以认为 m 内部是一个一个的一维数组
 * 每一个一维数组就是一个对象, 也就是线段
 *
 * 根据每一个线段的开始位置排序
 * m = { {5,7}, {1,4}, {2,6} }
 *
 * sorted : { {1,4}, {2,6}, {5,7} }
 *
 */
int comp(const void *a, const void *b)
{
	/* [1] -> *((int *)a + 1) */
	return *(int *)a - *(int *)b;
}

int cover_max_2(int m[][2], int rows)
{
	qsort(m, rows, sizeof(m[0]), comp);

	/* 小根堆 */
	std::priority_queue < int, std::vector < int >, std::greater < int > > heap;
	int max = 0;

	for (int i = 0; i < rows; i++) {
		int *line = m[i];

		while (!heap.empty() && heap.top() <= line[0]) {
			heap.pop();
		}

		heap.push(line[1]);

		max = std::max((unsigned long)max, heap.size());
	}

	return max;
}

int create_random_lines(int n, int l, int r, int lines[][2], int *actual_size)
{
	int size = (int)(RANDOM() * n) + 1;
	*actual_size = size;

	for (int i = 0; i < size; i++) {
		int a = l + (int)(RANDOM() * (r - l + 1));
		int b = l + (int)(RANDOM() * (r - l + 1));
		if (a == b) {
			b = a + 1;
		}

		lines[i][0] = std::min(a, b);
		lines[i][1] = std::max(a, b);
	}

	return 0;
}

int main(int argc, char *argv[])
{
	int n = 10;
	int l = 0;
	int r = 10;
	int times = 1000;
	bool succeed = true;

	srandom((unsigned) time(0));

	for (int i = 0; i < times; i++) {
		int actual_size = 0;
		int lines[n][2];
		memset(lines, 0, sizeof(lines));

		create_random_lines(n, l, r, lines, &actual_size);

		int res1 = cover_max_1(lines, actual_size);
		int res2 = cover_max_2(lines, actual_size);
#if 1
		for (int i = 0; i < actual_size; i++) {
			printf("(%2d, %2d), ", lines[i][0], lines[i][1]);
		}
		printf("\n");
		printf("cover max: %d, %d\n\n", res1, res2);
#endif

		if (res1 != res2) {
			succeed = false;
			break;
		}
	}

	printf("result: %s\n", succeed ? "nice" : "fucked");

	return 0;
}
