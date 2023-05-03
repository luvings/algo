
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include <queue>
#include <stack>

/* [0, 1) */
#define RANDOM()    (random() / (RAND_MAX + 1.0))

typedef struct two_queue_impl_stack m_stack_t;

struct two_queue_impl_stack {
	void (*push)(m_stack_t * s, int num);
	int (*pop)(m_stack_t * s);
	int (*peek)(m_stack_t * s);
	bool (*empty)(m_stack_t * s);

	std::queue < int > queue;
	std::queue < int > help;
};

void push(m_stack_t * s, int num)
{
	s->queue.push(num);
}

/*
 * front(), back(): 如果队列容器为空, 则会导致未定义的行为
 */
int pop(m_stack_t * s)
{
	int rval = INT_MAX;

	if (s->queue.empty()) {
		printf("m_stack_t empty\n");
		return rval;
	}

	while (s->queue.size() > 1) {
		int t = s->queue.front();
		s->queue.pop();

		s->help.push(t);
	}

	rval = s->queue.front();
	s->queue.pop();

	std::queue < int > tq = s->queue;
	s->queue = s->help;
	s->help = tq;

	return rval;
}

int peek(m_stack_t * s)
{
	int rval = INT_MAX;

	if (s->queue.empty()) {
		printf("m_stack_t empty\n");
		return rval;
	}

	while (s->queue.size() > 1) {
		int t = s->queue.front();
		s->queue.pop();

		s->help.push(t);
	}

	rval = s->queue.front();
	s->queue.pop();

	s->help.push(rval);

	std::queue < int > tq = s->queue;
	s->queue = s->help;
	s->help = tq;

	return rval;
}

bool empty(m_stack_t * s)
{
	return s->queue.empty();
}

int main(int argc, char *argv[])
{
	int max_value = 100;
	int times = 10000;
	bool succeed = true;

	srandom((unsigned) time(0));

	m_stack_t m_stk = {
		.push = push,
		.pop = pop,
		.peek = peek,
		.empty = empty,
	};
	std::stack < int > sys_stk;

	for (int i = 0; i < times; i++) {
		if (m_stk.empty(&m_stk)) {
			if (!sys_stk.empty()) {
				printf("oops..1\n");
				succeed = false;
				break;
			}

			int num = (int)(RANDOM() * (max_value + 1));
			m_stk.push(&m_stk, num);
			sys_stk.push(num);
		} else {
			if (RANDOM() < 0.25) {
				int num = (int)(RANDOM() * (max_value + 1));
				m_stk.push(&m_stk, num);
				sys_stk.push(num);
			} else if (RANDOM() < 0.5) {
				if (m_stk.peek(&m_stk) != sys_stk.top()) {
					printf("oops..2\n");
					succeed = false;
					break;
				}
			} else if (RANDOM() < 0.75) {
				if (m_stk.pop(&m_stk) != sys_stk.top()) {
					printf("oops..3\n");
					succeed = false;
					break;
				}
				sys_stk.pop();
			} else {
				if (m_stk.empty(&m_stk) != sys_stk.empty()) {
					printf("oops..4\n");
					succeed = false;
					break;
				}
			}
		}
	}

	printf("result: %s\n", succeed ? "nice" : "fucked");

	return 0;
}
