
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <algorithm>
#include <iostream>
#include <vector>

/* [0, 1) */
#define RANDOM()    (random() / (RAND_MAX + 1.0))

int process_vio(std::vector < int > vec, uint32_t idx, int rest)
{
	if (rest < 0) {
		return 0;
	}

	if (idx == vec.size()) {
		return rest == 0 ? 1 : 0;
	}

	int ways = 0;

	for (int zh = 0; zh * vec[idx] <= rest; zh++) {
		ways += process_vio(vec, idx + 1, rest - zh * vec[idx]);
	}

	return ways;
}

int ways_vio(std::vector < int > vec, int aim)
{
	if (vec.size() == 0 || aim < 0) {
		return 0;
	}

	return process_vio(vec, 0, aim);
}

/*
 * 如果 index 和 rest 的参数组合, 是没算过的, dp[index][rest] == -1
 * 如果 index 和 rest 的参数组合, 是算过的, dp[index][rest] > - 1
 */
int process_cache(std::vector < int > vec, uint32_t idx, int rest, int *dp, int line)
{
	if (*(dp + idx * line + rest) != -1) {
		return *(dp + idx * line + rest);
	}

	int ways = 0;

	if (idx == vec.size()) {
		ways = (rest == 0) ? 1 : 0;
	} else {
		for (int zh = 0; zh * vec[idx] <= rest; zh++) {
			ways += process_cache(vec, idx + 1, rest - (zh * vec[idx]), dp, line);
		}
	}

	/* dp[index][rest] = ways; */
	*(dp + idx * line + rest) = ways;

	return ways;
}

int ways_cache(std::vector < int > vec, int aim)
{
	if (vec.size() == 0 || aim < 0) {
		return 0;
	}

	/*
	 * std::map < std::string, int > map;
	 *
	 * 一开始所有的过程, 都没有计算呢
	 * dp[..][..]  = -1
	 */
	int dp[vec.size() + 1][aim + 1];
	int line = aim + 1;
	memset(dp, -1, sizeof(dp));

	return process_cache(vec, 0, aim, &dp[0][0], line);
}

int ways_dp(std::vector < int > vec, int aim)
{
	if (vec.size() == 0 || aim < 0) {
		return 0;
	}

	int n = vec.size();
	int dp[n + 1][aim + 1];
	memset(dp, 0, sizeof(dp));
	dp[n][0] = 1;		/* dp[N][1...aim] = 0; */

	for (int idx = n - 1; idx >= 0; idx--) {
		for (int rest = 0; rest <= aim; rest++) {
			int ways = 0;

			for (int zh = 0; zh * vec[idx] <= rest; zh++) {
				ways += dp[idx + 1][rest - (zh * vec[idx])];
			}

			dp[idx][rest] = ways;
		}
	}

	return dp[0][aim];
}

/*
 *          { 3, 2, 1, 4, }, aim = 8;
 *
 *
 *              0   1   2   3   4   5   6   7   8   -> rest
 *          0 | 1 | 1 | 2 | 3 | 5 | 6 | 9 | 11| 15|
 *          1 | 1 | 1 | 2 | 2 | 4 | 4 | 6 | 6 | ? |
 *          2 | 1 | 1 | 1 | 1 | 2 | 2 | 2 | 2 | v |
 *          3 | 1 | 0 | 0 | 0 | 1 | 0 | 0 | 0 | 1 |
 *          4 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
 *          ^
 *         idx
 *
 * such as:
 *      idx = 1;    // ways += dp[idx + 1][rest - (zh * vec[idx])]
 *
 *      dp[1][8] = dp[2][8] + dp[2][6] + dp[2][4] + dp[2][2] + dp[2][0];
 *      ..
 *      dp[1][6] =    #       dp[2][6] + dp[2][4] + dp[2][2] + dp[2][0];
 *
 */
int ways_dp_optimize(std::vector < int > vec, int aim)
{
	if (vec.size() == 0 || aim < 0) {
		return 0;
	}

	int n = vec.size();
	int dp[n + 1][aim + 1];
	memset(dp, 0, sizeof(dp));
	dp[n][0] = 1;		/* dp[N][1...aim] = 0; */

	for (int idx = n - 1; idx >= 0; idx--) {
		for (int rest = 0; rest <= aim; rest++) {
			dp[idx][rest] = dp[idx + 1][rest];

			if (rest - vec[idx] >= 0) {
				dp[idx][rest] += dp[idx][rest - vec[idx]];
			}
		}
	}

	return dp[0][aim];
}

std::vector < int > create_random_array(int max_size, int max_value)
{
	int size = (int)(RANDOM() * (max_size + 1));
	std::vector < int > vec;

	bool has[max_value];
	memset(has, 0, sizeof(has));

	for (int i = 0; i < size; i++) {
		int val = 0;
		do {
			val = (int)(RANDOM() * max_value) + 1;
		} while (has[val]);

		has[val] = true;
		vec.push_back(val);

	}

	return vec;
}

template < typename T > void fun(T val)
{
	std::cout.width(4);
	std::cout << val << ", ";
}

int main(int argc, char **argv)
{
	int max_size = 10;
	int max_value = 30;
	int times = 5;
	bool succeed = true;

	srandom((unsigned) time(0));

	for (int i = 0; i < times; i++) {
		std::vector < int > coins = create_random_array(max_size, max_value);
		int aim = (int)(RANDOM() * max_value);
#if 0
		std::vector < int > coins = {
			3, 2, 1, 4,
		};
		int aim = 8;
#endif
		int ans1 = ways_vio(coins, aim);
		int ans2 = ways_cache(coins, aim);
		int ans3 = ways_dp(coins, aim);
		int ans4 = ways_dp_optimize(coins, aim);

		if (ans1 != ans2 || ans2 != ans3 || ans3 != ans4) {
			for_each(coins.begin(), coins.end(), fun < int >);
			std::cout << std::endl;
			printf("aim = %d\n", aim);
			printf("%d,  %d,  %d,  %d", ans1, ans2, ans3, ans4);
			printf("oops");
			break;
		}
	}

	printf("result: %s\n", succeed ? "nice" : "fucked");

	return 0;
}
