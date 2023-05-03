
#include <stdio.h>

#include <iostream>
#include <algorithm>
#include <string>
#include <vector>

typedef struct student {
	int age;

	std::string name;
} m_student_t;

int sortf(const m_student_t & a, const m_student_t & b)
{
	return a.age > b.age;
}

void print_heap_s(std::vector < struct student > &v)
{
	for (std::vector < m_student_t >::iterator it = v.begin(); it != v.end(); it++) {
		std::cout << "[ " << it->age << ", '" << it->name << "' ], ";
	}
	std::cout << std::endl << std::endl;
}

int main(int argc, char *argv[])
{
	std::vector < struct student > v1;

	std::cout << "v1.size(): " << v1.size() << std::endl;

	m_student_t s1 = { 18, "A stu" };
	m_student_t s2 = { 11, "B stu" };
	m_student_t s3 = { 32, "C stu" };
	m_student_t s4 = { 14, "D stu" };
	v1.push_back(s1);
	v1.push_back(s2);
	v1.push_back(s3);
	v1.push_back(s4);

	auto sortf_l =[](const m_student_t & a, const m_student_t & b) {
		return a.age > b.age;
	};

	/* 排序策略是年龄小的放在堆顶, 年龄小根堆 */
	make_heap(v1.begin(), v1.end(), sortf_l);
	print_heap_s(v1);

	/* s1 'A stu' 年龄从 18 改成了 4 */

	/*
	 * 此时堆被破坏了, 因为你擅自改了一个同学的年龄
	 * 需要方法, 能让堆恢复成年龄小根堆
	 * 系统提供的堆做不到的
	 */
	s1.age = 4;
    printf("after 'A stu'.age -> 4,\n\n");

	print_heap_s(v1);

	return 0;
}
