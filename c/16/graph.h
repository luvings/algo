
#ifndef __GRAPH_H__
#define __GRAPH_H__

#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <map>
#include <queue>
#include <set>
#include <stack>
#include <vector>

#define ROWS            4
#define COLS            3

#define ARRAY_SIZE(x)   (sizeof(x) / sizeof((x)[0]))

#define NEW_NODE(type, var)   ({                    \
    type *__ptr = (type *) malloc(sizeof(type));    \
    assert(__ptr);                                  \
    memset((void *)__ptr, 0, sizeof(type));         \
    __ptr->val   = (var);                           \
    __ptr->in    = 0;                               \
    __ptr->out   = 0;                               \
    __ptr;                                          \
})

#define NEW_EDGE(type, wt, fr, tn)   ({             \
    type *__ptr = (type *) malloc(sizeof(type));    \
    assert(__ptr);                                  \
    memset((void *)__ptr, 0, sizeof(type));         \
    __ptr->weight = (wt);                           \
    __ptr->from   = (fr);                           \
    __ptr->to     = (tn);                           \
    __ptr;                                          \
})

struct node {
	int val;
	int in;
	int out;

	std::vector < struct node * > nexts;
	std::vector < struct edge * > edges;
};

struct edge {
	int weight;

	struct node *from;
	struct node *to;
};

struct graph {
	std::map < int, struct node * > nodes;
	std::set < struct edge * > edges;
};

struct graph create_entire_graph(int matrix[][COLS], int rows, int cols);

void destroy_entire_graph(struct graph *graph);

void print_entire_graph(struct graph *graph);

#endif /* __GRAPH_H__ */

