
#include <stdio.h>
#include <stdint.h>

#include <string>
#include <list>

void swap(std::string &s, int i, int j)
{
	char c = s[i];
	s[i] = s[j];
	s[j] = c;
}

void process(std::string s, uint32_t idx, std::list < std::string > &res)
{
	if (idx == s.length()) {
		res.push_back(s);
		return;
	} else {
		for (uint32_t i = idx; i < s.length(); i++) {
			swap(s, i, idx);

			process(s, idx + 1, res);

			swap(s, i, idx);
		}
	}
}

std::list < std::string > permutations(std::string s)
{
	std::list < std::string > res;

	if (s.length() == 0) {
		return res;
	}

	process(s, 0, res);

	return res;
}

void process_boundary(std::string s, uint32_t idx, std::list < std::string > &res)
{
	if (idx == s.length()) {
		res.push_back(s);
		return;
	} else {
		bool visited[256] = { false, };

		for (uint32_t i = idx; i < s.length(); i++) {
			if (!visited[(s[i] - '0')]) {
				visited[(s[i] - '0')] = true;

				swap(s, i, idx);
				process_boundary(s, idx + 1, res);
				swap(s, i, idx);
			}
		}
	}
}

std::list < std::string > permutations_boundary(std::string s)
{
	std::list < std::string > res;

	if (s.length() == 0) {
		return res;
	}

	process_boundary(s, 0, res);

	return res;
}

int main(int argc, char **argv)
{
	std::string s = "aac";

	std::list < std::string > res = permutations(s);

	printf("permutations:\t");
    for (auto it : res) {
		printf("%s,  ", it.c_str());
	}
	printf("\n");

	std::list < std::string > res_b = permutations_boundary(s);

	printf("permutations boundary:\t");
    for (auto it : res_b) {
		printf("%s,  ", it.c_str());
	}
	printf("\n");

	return 0;
}
