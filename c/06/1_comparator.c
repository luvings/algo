
#include <stdio.h>

#include <algorithm>
#include <vector>

/*
* JAVA 比较器 compare 方法里, 遵循一个统一的规范
*
* 1) 返回负数的时候, 认为第一个参数应该排在前面
* 2) 返回正数的时候, 认为第二个参数应该排在前面
* 3) 返回 0 时, 认为无所谓谁放前面
*/
int compare(int id1, int id2)
{
	return id1 - id2;
}

/*
 * c++
 *
 * a < b, 默认的升序排列
 *
 * a > b, 降序排列
 */
bool greater(int a, int b)
{
	return a < b;
}

bool decrease(int a, int b)
{
	return a > b;
}

int main(int argc, char *argv[])
{
	std::vector < int > v;

	v.push_back(3);
	v.push_back(1);
	v.push_back(5);

	std::sort(v.begin(), v.end(), greater);
	std::sort(v.begin(), v.end(), decrease);

	for (std::vector < int >::iterator it = v.begin(); it != v.end(); it++) {
		printf("%d, ", *it);
	}
	printf("\n");

	return 0;
}
