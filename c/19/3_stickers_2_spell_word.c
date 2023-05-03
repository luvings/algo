
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <string>
#include <map>

#define ARRAY_SIZE(x)   (sizeof(x) / sizeof((x)[0]))

/*
 * stickers[i] 数组, 当初 i 号贴纸的字符统计 int stickers[][26] -> 所有的贴纸
 * 每一种贴纸都有无穷张
 * 返回搞定 target 的最少张数
 * 最少张数
 */
int process(int stickers[][26], int n, std::string rest)
{
	/* rest 为空, 说明搞定了, 还需要 0 张贴纸 */
	if (rest.length() == 0) {
		return 0;
	}

	int count[26] = { 0, };
    for (char c : rest) {
		count[c - 'a']++;
	}

	int min = INT_MAX;

	for (int i = 0; i < n; i++) {
		/* 尝试第一张贴纸是谁 */
		int *first = stickers[i];
		/*
		 * 最关键的优化(重要的剪枝!这一步也是贪心!)
		 *
		 * 如果没有此判断, 也会造成栈溢出
		 *
		 * { "aa", "bb", }, rest = "xyz"
		 * 则一路调用 process 递归, 无法返回
		 */
		if (first[rest[0] - 'a'] > 0) {
			std::string builder;

			for (int j = 0; j < 26; j++) {
				if (count[j] > 0) {
					int nums = count[j] - first[j];
					for (int k = 0; k < nums; k++) {
						builder.push_back((char)(j + 'a'));
					}
				}
			}

			min = std::min(min, process(stickers, n, builder));
		}
	}

	return min + (min == INT_MAX ? 0 : 1);
}

int min_stickers(std::string stickers[], int n, std::string target)
{
	int counts[n][26];
	memset(counts, 0, sizeof(counts));

	/* 关键优化(用词频表替代贴纸数组) */
	for (int i = 0; i < n; i++) {
		std::string first = stickers[i];

	    for (char c : first) {
			counts[i][c - 'a']++;
		}
	}

	int ans = process(counts, n, target);

	return ans == INT_MAX ? -1 : ans;
}

int process_dp(int stickers[][26], int n, std::string rest, std::map < std::string, int > &dp)
{
	if (dp.find(rest) != dp.end()) {
		return dp.find(rest)->second;
	}

	int count[26] = { 0, };
    for (char c : rest) {
		count[c - 'a']++;
	}

	int min = INT_MAX;

	for (int i = 0; i < n; i++) {
		int *first = stickers[i];

		if (first[rest[0] - 'a'] > 0) {
			std::string builder;

			for (int j = 0; j < 26; j++) {
				if (count[j] > 0) {
					int nums = count[j] - first[j];
					for (int k = 0; k < nums; k++) {
						builder.push_back((char)(j + 'a'));
					}
				}
			}

			min = std::min(min, process_dp(stickers, n, builder, dp));
		}
	}

	int ans = min + (min == INT_MAX ? 0 : 1);
	dp.insert(std::make_pair(rest, ans));

	return ans;
}

int min_stickers_dp(std::string stickers[], int n, std::string target)
{
	int counts[n][26];
	memset(counts, 0, sizeof(counts));

	for (int i = 0; i < n; i++) {
		std::string first = stickers[i];

	    for (char c : first) {
			counts[i][c - 'a']++;
		}
	}

	std::map < std::string, int > dp;
	dp.insert(std::make_pair("", 0));

	int ans = process_dp(counts, n, target, dp);

	return ans == INT_MAX ? -1 : ans;
}

/*
 * 所有贴纸 stickers, 每一种贴纸都有无穷张
 * 最少张数
 */
std::string minus(std::string rest, std::string target)
{
	int count[26] = { 0, };

    for (char c : rest) {
		count[c - 'a']++;
	}
    for (char c : target) {
		count[c - 'a']--;
	}

	std::string builder;
	for (int i = 0; i < 26; i++) {
		if (count[i] > 0) {
			for (int j = 0; j < count[i]; j++) {
				builder.push_back((char)(i + 'a'));
			}
		}
	}

	return builder;
}

int process_vio(std::string stickers[], int n, std::string rest)
{
	if (rest.length() == 0) {
		return 0;
	}

	int min = INT_MAX;

	for (int i = 0; i < n; i++) {
		std::string first = stickers[i];
		std::string next = minus(rest, first);

		if (next.length() != rest.length()) {
			min = std::min(min, process_vio(stickers, n, next));
		}
	}

	return min + (min == INT_MAX ? 0 : 1);
}

int min_stickers_vio(std::string stickers[], int n, std::string target)
{
	int res = process_vio(stickers, n, target);

	return res == INT_MAX ? -1 : res;
}

int main(int argc, char **argv)
{
	std::string stickers[] = {
		"aaaa", "bbaa", "ccddd",
	};
	std::string target = "abcccccdddddbbbaaaaa";

	printf("%d\n", min_stickers_vio(stickers, ARRAY_SIZE(stickers), target));
	printf("%d\n", min_stickers(stickers, ARRAY_SIZE(stickers), target));
	printf("%d\n", min_stickers_dp(stickers, ARRAY_SIZE(stickers), target));

	return 0;
}
