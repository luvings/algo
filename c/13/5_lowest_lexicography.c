
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <iostream>
#include <algorithm>
#include <string>
#include <set>
#include <vector>

/* [0, 1) */
#define RANDOM()    (random() / (RAND_MAX + 1.0))

std::string lowest_lexicography(std::vector < std::string > strs)
{
	if (strs.empty() || strs.size() == 0) {
		return "";
	}

	sort(strs.begin(), strs.end(),[](std::string a, std::string b)->bool {
	     return (a.append(b)).compare(b.append(a)) < 0;
	     }
	);

	std::string res = "";
	for (uint32_t i = 0; i < strs.size(); i++) {
		res.append(strs.at(i));
	}

	return res;
}

void process(std::vector < std::string > strs, std::set < int > used, std::string path, std::vector < std::string > &all)
{
	if (used.size() == strs.size()) {
		all.push_back(path);
	} else {
		for (uint32_t i = 0; i < strs.size(); i++) {
			if (!used.count(i)) {
				used.insert(i);

				process(strs, used, path.append(strs.at(i)), all);

				for (uint32_t j = 0; j < strs.at(i).length(); j++) {
					path.pop_back();
				}
				used.erase(i);
			}
		}
	}
}

std::string lowest_lexicography_cmp(std::vector < std::string > strs)
{
	if (strs.empty() || strs.size() == 0) {
		return "";
	}

	std::vector < std::string > all;
	std::set < int > used;

	process(strs, used, "", all);

	std::string lowest = all.at(0);
	for (uint32_t i = 0; i < all.size(); i++) {
		if (all.at(i).compare(lowest) < 0) {
			lowest = all.at(i);
		}
	}

	return lowest;
}

int create_random_strings_array(int max_size, int max_length, std::vector < std::string > &v)
{
	int size = (int)(RANDOM() * (max_size + 1));

	for (int i = 0; i < size; i++) {
		std::string str = "";
		int length = (int)(RANDOM() * (max_length + 1));

		for (int j = 0; j < length; j++) {
			int val = (int)(RANDOM() * 5);
			str.push_back((RANDOM() <= 0.5) ? (char)(65 + val) : (char)(97 + val));
		}

		if (str.length() != 0) {
			v.push_back(str);
		}
	}

	return 0;
}

template < typename T > void fun(T val)
{
	std::cout << val << ",  ";
}

int main(int argc, char **argv)
{
	int max_size = 6;
	int max_length = 5;
	int times = 5;
	bool succeed = true;

	srandom((unsigned) time(0));

	for (int i = 0; i < times; i++) {
		std::vector < std::string > strs;

		create_random_strings_array(max_size, max_length, strs);

		if (lowest_lexicography(strs) != lowest_lexicography_cmp(strs)) {
			printf("strs (%ld):\t", strs.size());
			for_each(strs.begin(), strs.end(), fun < std::string >);
			std::cout << std::endl;

			printf("lowest lexicography: %s,   %s\n", lowest_lexicography(strs).c_str(),
			       lowest_lexicography_cmp(strs).c_str());
			printf("oops\n");
			succeed = false;
			break;
		}

	}

	printf("result: %s\n", succeed ? "nice" : "fucked");

	return 0;
}
