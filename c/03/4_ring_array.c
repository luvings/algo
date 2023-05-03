
#include <limits.h>
#include <stdio.h>
#include <stdbool.h>

#define TTY_BUF_SIZE    16

#define INC(a)      ((a) = ((a)+1) & (TTY_BUF_SIZE-1))
#define DEC(a)      ((a) = ((a)-1) & (TTY_BUF_SIZE-1))

#define EMPTY(a)    ((a)->head == (a)->tail)

#define CHARS(a)    (((a)->head-(a)->tail)&(TTY_BUF_SIZE-1))

#define LEFT(a)     (((a)->tail-(a)->head-1)&(TTY_BUF_SIZE-1))
#define FULL(a)     (!LEFT(a))

#define LAST(a)     ((a)->buf[(TTY_BUF_SIZE-1)&((a)->head-1)])

#define GETCH(queue, c) \
    (void) ({ c = (queue)->buf[(queue)->tail];  \
              INC((queue)->tail); })

#define PUTCH(c, queue) \
    (void) ({ (queue)->buf[(queue)->head] = (c);    \
              INC((queue)->head); })


#define QLIMIT          4

typedef struct queue {
	int buf[QLIMIT];
	int size;

	int head;
	int tail;
} m_queue_t;

int next_idx(int idx)
{
	return idx < (QLIMIT - 1) ? idx + 1 : 0;
}

void push(m_queue_t * q, int val)
{
	if (q->size == QLIMIT) {
		printf("queue full\n");
		return;
	}

	q->size++;
	q->buf[q->head] = val;
	q->head = next_idx(q->head);	// if not overflow, head++;
}

int pop(m_queue_t * q)
{
	int rval = INT_MAX;

	if (q->size == 0) {
		printf("queue empty\n");
		return rval;
	}

	q->size--;
	rval = q->buf[q->tail];
	q->tail = next_idx(q->tail);	// if not overflow, tail++;

	return rval;
}

bool empty(m_queue_t * q)
{
	return q->size == 0;
}

int main(int argc, char *argv[])
{
	m_queue_t q = {
		.buf = { 0, },
		.size = 0,
		.head = 0,
		.tail = 0,
	};

	push(&q, 0);
	push(&q, 1);
	push(&q, 2);
	push(&q, 3);
	push(&q, 4);

	pop(&q);
	pop(&q);

	push(&q, 4);
	push(&q, 5);
	push(&q, 6);

	pop(&q);
	pop(&q);
	pop(&q);
	pop(&q);
	pop(&q);

	int a = 0;

	DEC(a);
	printf("%2d = %2d(0x%02x)\n", -1, a, a);

	a = -2;
	DEC(a);
	printf("%2d = %2d(0x%02x)\n", -3, a, a);

	a = 15;
	INC(a);
	printf("%2d = %2d(0x%02x)\n", 16, a, a);

	return 0;
}
