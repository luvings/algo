
#include "graph.h"

/*
 * matrix 所有的边
 * N * 3 的矩阵
 * [ weight, from 节点编号, to 节点编号 ]
 *
 * [ 5 , 0, 7 ]
 * [ 3 , 0, 1 ]
 */
struct graph create_entire_graph(int matrix[][COLS], int rows, int cols)
{
	struct graph graph;

	for (int i = 0; i < rows; i++) {
		int weight = matrix[i][0];
		int f_node_id = matrix[i][1];
		int t_node_id = matrix[i][2];

		if (!graph.nodes.count(f_node_id)) {
			graph.nodes.insert(std::make_pair(f_node_id, NEW_NODE(struct node, f_node_id)));
		}
		if (!graph.nodes.count(t_node_id)) {
			graph.nodes.insert(std::make_pair(t_node_id, NEW_NODE(struct node, t_node_id)));
		}

		struct node *from = graph.nodes[f_node_id];
		struct node *to = graph.nodes[t_node_id];

		struct edge *edge = NEW_EDGE(struct edge, weight, from, to);

		from->out++;
		from->nexts.push_back(to);
		from->edges.push_back(edge);

		to->in++;

		graph.edges.insert(edge);
	}

	return graph;
}

void print_entire_graph(struct graph *graph)
{
	printf("entire graph has %ld nodes, %ld edges\n\n", graph->nodes.size(), graph->edges.size());

	for (auto it = graph->nodes.begin(); it != graph->nodes.end(); it++) {
		struct node *node = it->second;

		printf("(no. %2d): val = %2d, in = %2d, out = %2d\n", it->first, node->val, node->in, node->out);

		for (uint32_t j = 0; j < node->nexts.size(); j++) {
			printf("\t -> %2d,  ", node->nexts[j]->val);
			printf("(w %2d)\n", node->edges[j]->weight);
		}
	}
	printf("\n");

	for (std::set < struct edge * >::iterator it = graph->edges.begin(); it != graph->edges.end(); it++) {
		struct edge *edge = *it;

		printf("(e %2d): %2d -> %2d\n", edge->weight, edge->from->val, edge->to->val);
	}

	printf("\n---8<--------------------------------------------------\n\n");
}

void destroy_entire_graph(struct graph *graph)
{
	for (auto it = graph->nodes.begin(); it != graph->nodes.end(); it++) {
		free(it->second);
	}

	for (auto it = graph->edges.begin(); it != graph->edges.end(); it++) {
		free(*it);
	}

	graph->nodes.clear();
	graph->edges.clear();
}

