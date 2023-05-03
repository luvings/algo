
#include "graph.h"

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

std::set < struct edge * > prim_mst(struct graph *graph)
{
	/*
	 * @q: 解锁的边进入小根堆
	 * @unlock_nodes: 哪些点被解锁出来了
	 * @s_edges: 已经考虑的边, 不在重复考虑
	 * @result:
	 */
	std::priority_queue < struct edge *, std::vector < struct edge * >, cmp > q;

	std::set < struct node * > unlock_nodes;
	std::set < struct edge * > s_edges;
	std::set < struct edge * > result;

	/* for 为了兼顾不连通的森林图 */
    for (auto it : graph->nodes) {
		/* 随便选择一个点, cnode 是开始点 */
		struct node *cnode = it.second;

		if (!unlock_nodes.count(cnode)) {
			unlock_nodes.insert(cnode);

			/* 由一个点, 解锁所有相连的边 */
		    for (auto e : cnode->edges) {
				if (!s_edges.count(e)) {
					s_edges.insert(e);
					q.push(e);
				}
			}

			while (!q.empty()) {
				/* 弹出解锁的边中, 最小的边 */
				struct edge *cedge = q.top();
				q.pop();

				/* 可能的一个新的点, 不含有的时候, 就是新的点 */
				struct node *n_to = cedge->to;
				if (!unlock_nodes.count(n_to)) {
					unlock_nodes.insert(n_to);

					result.insert(cedge);

				    for (auto e : n_to->edges) {
						if (!s_edges.count(e)) {
							s_edges.insert(e);
							q.push(e);
						}
					}
				}
			}
		}
		//break;
	}

	printf("prim mst: ");
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

	std::set < struct edge * > v = prim_mst(&graph);

	destroy_entire_graph(&graph);

	return 0;
}
