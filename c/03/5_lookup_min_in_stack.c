
#include <limits.h>
#include <stdio.h>

#include <stack>

typedef struct stack {
	std::stack < int > s_data;
	std::stack < int > s_min;
} m_stack_t;

int get_stakc_min(m_stack_t * s)
{
	if (s->s_min.empty()) {
		printf("s_min empty\n");
		return INT_MAX;
	}

	return s->s_min.top();
}

void push(m_stack_t * s, int num)
{
	if (s->s_min.empty() || num <= get_stakc_min(s)) {
		s->s_min.push(num);
	}

	s->s_data.push(num);
}

int pop(m_stack_t * s)
{
	int rval = INT_MAX;

	if (s->s_data.empty()) {
		printf("s_data empty\n");
		return rval;
	}

	rval = s->s_data.top();
	s->s_data.pop();

	if (rval == get_stakc_min(s)) {
		s->s_min.pop();
	}

	return rval;
}

int main(int argc, char *argv[])
{
	m_stack_t s;

	push(&s, 3);
	printf("min val = %d\n", get_stakc_min(&s));
	push(&s, 2);
	printf("min val = %d\n", get_stakc_min(&s));
	push(&s, 4);
	printf("min val = %d\n", get_stakc_min(&s));
	push(&s, 1);
	printf("min val = %d\n", get_stakc_min(&s));

	pop(&s);
	printf("min val = %d\n", get_stakc_min(&s));

	return 0;
}
