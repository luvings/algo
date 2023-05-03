
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

struct program {
	int start;
	int end;
};

int meeting_arrange_1(std::vector < struct program > progs)
{
	if (progs.size() == 0) {
		return 0;
	}

	sort(progs.begin(), progs.end(),[](struct program a, struct program b)->bool {
	     return a.end < b.end;
	     }
	);

	int time_line = 0;
	int result = 0;

	for (uint32_t i = 0; i < progs.size(); i++) {
		if (time_line <= progs[i].start) {
			result++;
			time_line = progs[i].end;
		}
	}

	return result;
}

/*
 * 错误的贪心策略
 *
 * 1. 会议开始时间先安排
 * 2. 会议持续时间短的先安排
 * ...
 *
 */
int meeting_arrange_2(std::vector < struct program > progs)
{
	if (progs.size() == 0) {
		return 0;
	}

	sort(progs.begin(), progs.end(),[](struct program a, struct program b)->bool {
	     return a.start < b.start;
	     }
	);

	int time_line = 0;
	int result = 0;

	for (uint32_t i = 0; i < progs.size(); i++) {
		if (time_line <= progs[i].start) {
			result++;
			time_line = progs[i].end;
		}
	}

	return result;
}

/*
 * 还剩下的会议都放在 progs 里
 * done 之前已经安排了多少会议的数量
 * timeLine 目前来到的时间点是什么
 *
 * 目前来到 timeLine 的时间点, 已经安排了 done 多的会议, 剩下的会议 progs 可以自由安排
 * 返回能安排的最多会议数量
*/
std::vector < struct program > copy_but_except(std::vector < struct program > progs, uint32_t idx)
{
	std::vector < struct program > remain_progs;

	for (uint32_t i = 0; i < progs.size(); i++) {
		if (i != idx) {
			remain_progs.push_back(progs.at(i));
		}
	}

	return remain_progs;
}

int process_cmp(std::vector < struct program > progs, int done, int time_line)
{
	if (progs.size() == 0) {
		return done;
	}

	int max = done;

	for (uint32_t i = 0; i < progs.size(); i++) {
		if (progs[i].start >= time_line) {
			std::vector < struct program > nexts = copy_but_except(progs, i);
			max = std::max(max, process_cmp(nexts, done + 1, progs[i].end));
		}
	}

	return max;
}

int meeting_arrange_cmp(std::vector < struct program > progs)
{
	if (progs.size() == 0) {
		return 0;
	}

	return process_cmp(progs, 0, 0);
}

std::string space(int n)
{
	std::string s;

	s.resize(n, ' ');

	return s;
}

void fun(struct program prog)
{
	std::cout.width(4);
	std::cout << space(prog.start) << prog.start << space(prog.end - prog.start) << prog.end << std::endl;
}

std::vector < struct program > create_random_programs(int max_programs, int limit_time)
{
	int size = (int)(RANDOM() * (max_programs + 1));
	std::vector < struct program > progs_vec;

	for (int i = 0; i < size; i++) {
		struct program prog = { 0, };
		int r1 = (int)(RANDOM() * (limit_time + 1));
		int r2 = (int)(RANDOM() * (limit_time + 1));

		if (r1 == r2) {
			prog.start = r1;
			prog.end = r1 + 1;
		} else {
			prog.start = std::min(r1, r2);
			prog.end = std::max(r1, r2);
		}

		progs_vec.push_back(prog);
	}

	return progs_vec;
}

int main(int argc, char **argv)
{
	int max_programs = 12;
	int limit_time = 20;
	int times = 5;
	bool succeed = true;

	srandom((unsigned) time(0));

	for (int i = 0; i < times; i++) {
		std::vector < struct program > progs = create_random_programs(max_programs, limit_time);

		if (meeting_arrange_1(progs) != meeting_arrange_cmp(progs)) {
			for_each(progs.begin(), progs.end(), fun);
			printf("progs.size() = %ld\n", progs.size());
			printf("max meeting arrangement count: %d, but failure count: %d\n", meeting_arrange_cmp(progs),
			       meeting_arrange_1(progs));
			printf("---8<---------------------------------------------------\n\n");
			succeed = false;
			break;
		}
	}

	printf("result: %s\n", succeed ? "nice" : "fucked");

	return 0;
}
