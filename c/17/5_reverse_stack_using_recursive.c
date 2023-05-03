
#include <stdio.h>

#include <stack>

/*
 * 栈底元素移除掉
 * 上面的元素盖下来
 * 返回移除掉的栈底元素
 */
int f(std::stack < int > &s)
{
	int result = s.top();
	s.pop();

	if (s.empty()) {
		return result;
	} else {
		int last = f(s);
		s.push(result);
		return last;
	}
}

void reverse_s(std::stack < int > &s)
{
	if (s.empty()) {
		return;
	}

	int r = f(s);
	reverse_s(s);
	s.push(r);
}

int main(int argc, char **argv)
{
	std::stack < int > s;
	s.push(1);
	s.push(2);
	s.push(3);
	s.push(4);

	reverse_s(s);

	while (!s.empty()) {
		printf("%2d,  ", s.top());
		s.pop();
	}
	printf("\n");

	return 0;
}
