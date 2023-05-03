
#include <stdio.h>

bool m_sum(int num)
{
	for (int start = 1; start <= num; start++) {
		int sum = start;
		for (int j = start + 1; j <= num; j++) {
			if (sum + j > num) {
				break;
			}

			if (sum + j == num) {
				return true;
			}

			sum += j;
		}
	}

	return false;
}

/*
 *   m & (m - 1) == 0  -> m 是 2 的 n 次方
 *
 *  (num & (~num + 1)) -> 提取出最右的 1
 *
 */

bool m_sum_awesome(int num)
{
	return (num & (num - 1)) != 0;
}

int main(int argc, char **argv)
{
	for (int i = 1; i < 200; i++) {
		printf("%4d : %4d\n", i, m_sum(i));
	}

	return 0;
}
