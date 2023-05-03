
#include "graph.h"

/*---8>-----------------------------------------------------------------------*/

struct union_find {
	std::map < void *, void * > father;
	std::map < void *, int > size;
};

void *find_father(struct union_find *u_find, void *cur)
{
	std::stack < void * > s_path;

	while (u_find->father.size() && cur != u_find->father.find(cur)->second) {
		s_path.push(cur);
		cur = u_find->father.find(cur)->second;
	}

	while (!s_path.empty()) {
		void *p = s_path.top();
		s_path.pop();

		u_find->father[p] = cur;
	}

	return cur;
}

bool is_same_set(struct union_find *u_find, void *a, void *b)
{
	if (!u_find->father.count(a) || !u_find->father.count(b)) {
		return false;
	}

	return find_father(u_find, a) == find_father(u_find, b);
}

void union_set(struct union_find *u_find, void *a, void *b)
{
	if (!u_find->father.count(a) || !u_find->father.count(b)) {
		return;
	}

	void *a_father = find_father(u_find, a);
	void *b_father = find_father(u_find, b);

	if (a_father != b_father) {
		int a_size = u_find->size.find(a_father)->second;
		int b_size = u_find->size.find(b_father)->second;

		void *big = a_size > b_size ? a_father : b_father;
		void *small = big == a_father ? b_father : a_father;

		u_find->father[small] = big;
		u_find->size[big] = a_size + b_size;
		u_find->size.erase(u_find->size.find(small));
	}
}

int set_init(struct union_find *u_find, std::vector < void * > nodes)
{
	u_find->father.clear();
	u_find->size.clear();

    for (auto e : nodes) {
		u_find->father.insert(std::make_pair(e, e));
		u_find->size.insert(std::make_pair(e, 1));
	}

	return 0;
}

/*---8<-----------------------------------------------------------------------*/

/*
 * undirected graph only
 *
 * mst -> Minimum Spanning Tree
 */
struct cmp {
	bool operator() (struct edge * a, struct edge * b) {
		return a->weight > b->weight;
	}
};

std::set < struct edge * > kruskal_mst(struct graph *graph)
{
	struct union_find u_find;
	std::vector < void * > nodes;

    for (auto it : graph->nodes) {
		nodes.push_back(it.second);
	}
	set_init(&u_find, nodes);

	/* 从小的边到大的边, 依次弹出, 小根堆 */
	std::priority_queue < struct edge *, std::vector < struct edge * >, cmp > q;
    for (auto e : graph->edges) {
		q.push(e);
	}

	std::set < struct edge * > result;

	while (!q.empty()) {
		struct edge *e = q.top();
		q.pop();

		if (!is_same_set(&u_find, e->from, e->to)) {
			result.insert(e);
			union_set(&u_find, e->from, e->to);
		}
	}

	printf("kruskal mst: ");
	int sum = 0;
    for (auto e : result) {
		printf("%2d, ", e->weight);
		sum += e->weight;
	}
	printf("sum = %2d\n", sum);

	return result;
}

/*
 *       1
 *      |  \
 *      |   \
 *      2 —— 3
 *      |   /
 *      |  /
 *       4
 *
 */
int main(int argc, char **argv)
{
	int matrix[][COLS] = {
		{ 2, 1, 2 },
		{ 2, 2, 1 },
		{ 9, 1, 3 },
		{ 9, 3, 1 },
		{ 8, 2, 3 },
		{ 8, 3, 2 },
		{ 7, 2, 4 },
		{ 7, 4, 2 },
		{ 3, 3, 4 },
		{ 3, 4, 3 },
	};

	struct graph graph = create_entire_graph(matrix, ARRAY_SIZE(matrix), COLS);

	print_entire_graph(&graph);

	std::set < struct edge * > v = kruskal_mst(&graph);

	destroy_entire_graph(&graph);

	return 0;
}
