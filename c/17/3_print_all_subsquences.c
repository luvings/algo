
#include <stdio.h>
#include <stdint.h>

#include <set>
#include <string>
#include <list>

/*
 * str 固定参数
 * 来到了 str[index] 字符, index 是位置
 * str[0..index-1] 已经走过了, 之前的决定, 都在 path 上
 * 之前的决定已经不能改变了, 就是 path
 * str[index....] 还能决定, 之前已经确定, 而后面还能自由选择的话,
 * 把所有生成的子序列, 放入到 ans 里去
 */
void process(std::string s, uint32_t idx, std::list < std::string > &res, std::string path)
{
	if (idx == s.length()) {
		res.push_back(path);
		return;
	}

	/* 没有要 index 位置的字符 */
	process(s, idx + 1, res, path);

	/* 要 index 位置的字符 */
	process(s, idx + 1, res, path + s.at(idx));
}

std::list < std::string > subs(std::string s)
{
	std::list < std::string > res;
	std::string path = "";

	process(s, 0, res, path);

	return res;
}

void process_no(std::string s, uint32_t idx, std::set < std::string > &res, std::string path)
{
	if (idx == s.length()) {
		res.insert(path);
		return;
	}

	process_no(s, idx + 1, res, path);

	process_no(s, idx + 1, res, path + s.at(idx));
}

std::list < std::string > subs_no_repeat(std::string s)
{
	std::set < std::string > set;
	std::string path = "";

	process_no(s, 0, set, path);

	std::list < std::string > res;
    for (auto it : set) {
		res.push_back(it);
	}

	return res;
}

int main(int argc, char **argv)
{
	std::string s = "abb";

	std::list < std::string > res = subs(s);
	std::list < std::string > res_n = subs_no_repeat(s);

	printf("subs:\t");
    for (auto it : res) {
		printf("%s,  ", it.c_str());
	}
	printf("\n");

	printf("subs without repetition:\t");
    for (auto it : res_n) {
		printf("%s,  ", it.c_str());
	}
	printf("\n");

	return 0;
}
