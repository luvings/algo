
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#include <string>

/* [0, 1) */
#define RANDOM()    (random() / (RAND_MAX + 1.0))

/*
 * str[0..i-1] 转化无需过问
 * str[i.....] 去转化, 返回有多少种转化方法
 */
int process(std::string s, uint32_t idx)
{
	if (idx == s.length()) {
		return 1;
	}

	/*
	 * idx 没到最后, 说明有字符
	 * 之前的决定有问题
	 */
	if (s[idx] == '0') {
		return 0;
	}

	/*
	 * str[i] != '0'
	 * 可能性一, i 单转
	 */
	int ways = process(s, idx + 1);

	if (idx + 1 < s.length() && (s[idx] - '0') * 10 + s[idx + 1] - '0' < 27) {
		ways += process(s, idx + 2);
	}

	return ways;
}

/*
 * str 只含有数字字符 0~9
 * 返回多少种转化方案
 */
int number(std::string s)
{
	if (s.length() == 0) {
		return 0;
	}

	return process(s, 0);
}

/*
 * 从右往左的动态规划
 * 就是上面方法的动态规划版本
 * dp[i] 表示: str[i...]有多少种转化方式
 */
int dp1(std::string s)
{
	if (s.length() == 0) {
		return 0;
	}

	int n = s.length();
	int dp[n + 1];
	memset(dp, 0, sizeof(dp));

	dp[n] = 1;

	for (int i = n - 1; i >= 0; i--) {
		if (s[i] != '0') {
			int ways = dp[i + 1];
			if (i + 1 < n && (s[i] - '0') * 10 + s[i + 1] - '0' < 27) {
				ways += dp[i + 2];
			}

			dp[i] = ways;
		}
	}

	return dp[0];
}

std::string create_random_string(int max_length)
{
	int length = (int)(RANDOM() * (max_length + 1));

	std::string str;

	for (int i = 0; i < length; i++) {
		str += (char)((int)(RANDOM() * 10) + '0');
	}

	return str;
}

int main(int argc, char **argv)
{
	int max_length = 30;
	int times = 5;
	bool succeed = true;

	srandom((unsigned) time(0));

	for (int i = 0; i < times; i++) {
		std::string s = create_random_string(max_length);

		if (number(s) != dp1(s)) {
			printf("s:  %s\n", s.c_str());
			printf("number(s) = %d, dp1(s) = %d\n", number(s), dp1(s));
			printf("oops\n");
			succeed = false;
			break;
		}
	}

	printf("result: %s\n", succeed ? "nice" : "fucked");

	return 0;
}
