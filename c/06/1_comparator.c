
#include <stdio.h>

/*
* 任何比较器
* compare方法里, 遵循一个统一的规范:
* 1) 返回负数的时候, 认为第一个参数应该排在前面
* 2) 返回正数的时候, 认为第二个参数应该排在前面
* 3) 返回0的时候, 认为无所谓谁放前面
*/
int compare(int id1, int id2)
{
	return id1 - id2;
}

int main(int argc, char *argv[])
{

	return 0;
}
