
#include "graph.h"

/*
 * 有向无环图 graph
 */
std::vector < struct node * > topology_sort(struct graph *graph)
{
	/* key 某个节点, value 剩余的入度 */
	std::map < struct node *, int > in_map;
	/* 只有剩余入度为 0 的点, 才进入 zero_q 队列 */
	std::queue < struct node * > zero_q;

    for (auto it : graph->nodes) {
		struct node *cnode = it.second;
		int in = it.second->in;

		in_map.insert(std::make_pair(cnode, in));

		if (in == 0) {
			zero_q.push(cnode);
		}
	}

	std::vector < struct node * > result;

	while (!zero_q.empty()) {
		struct node *cur = zero_q.front();
		zero_q.pop();

		result.push_back(cur);

	    for (auto next : cur->nexts) {
			in_map[next] -= 1;

			if (in_map[next] == 0) {
				zero_q.push(next);
			}
		}
	}

	printf("topology sort:\t");
    for (auto node : result) {
		printf("%2d, ", node->val);
	}
	printf("\n");

	return result;
}

/*
 *      1 —> 2 —> 3 —> 5   —>  6
 *                      \     /
 *                       v   v
 *                         4
 *
 */
int main(int argc, char **argv)
{
	int matrix[][COLS] = {
		{ 1, 1, 2 },
		{ 1, 2, 3 },
		{ 1, 3, 5 },
		{ 1, 5, 6 },
		{ 1, 3, 4 },
		{ 1, 6, 4 },
	};

	struct graph graph = create_entire_graph(matrix, ARRAY_SIZE(matrix), COLS);

	print_entire_graph(&graph);

	std::vector < struct node * > v = topology_sort(&graph);

	destroy_entire_graph(&graph);

	return 0;
}
