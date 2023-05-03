
#include <limits.h>
#include <stdio.h>

#include <stack>

typedef struct two_stack_impl_queue {
	std::stack < int > s_push;
	std::stack < int > s_pop;
} m_queue_t;

void push_2_pop(m_queue_t * q)
{
	if (q->s_pop.empty()) {
		while (!q->s_push.empty()) {
			int t = q->s_push.top();
			q->s_push.pop();

			q->s_pop.push(t);
		}
	}
}

void push(m_queue_t * q, int num)
{
	q->s_push.push(num);
	push_2_pop(q);
}

/*
 * top(): 如果堆栈容器为空, 则会导致未定义的行为
 */
int pop(m_queue_t * q)
{
	int rval = INT_MAX;

	if (q->s_push.empty() && q->s_pop.empty()) {
		printf("m_queue_t empty\n");
		return rval;
	}

	push_2_pop(q);

	rval = q->s_pop.top();
	q->s_pop.pop();

	return rval;
}

int peek(m_queue_t * q)
{
	if (q->s_push.empty() && q->s_pop.empty()) {
		printf("m_queue_t empty\n");
		return INT_MAX;
	}

	push_2_pop(q);

	return q->s_pop.top();
}

int main(int argc, char *argv[])
{
	m_queue_t q;

	push(&q, 1);
	push(&q, 2);
	push(&q, 3);
	printf("peek v = %d\n", peek(&q));
	printf("pop  v = %d\n", pop(&q));
	printf("peek v = %d\n", peek(&q));
	printf("pop  v = %d\n", pop(&q));
	printf("peek v = %d\n", peek(&q));
	printf("pop  v = %d\n", pop(&q));

	return 0;
}
