
#include "graph.h"

/*
 * no negative weight
 */
struct node *get_min_distance_node_from_unselected_nodes(std::map < struct node *, int > distance_map,
							 std::set < struct node * > selected_nodes)
{
	struct node *min_node = NULL;
	int min_distance = INT_MAX;

    for (auto it : distance_map) {
		struct node *node = it.first;
		int dist = it.second;

		if (!selected_nodes.count(node) && dist < min_distance) {
			min_node = node;
			min_distance = dist;
		}
	}

	return min_node;
}

std::map < struct node *, int > dijkstra(struct node *from)
{
	std::map < struct node *, int > distance_map;
	std::set < struct node * > selected_nodes;

	distance_map.insert(std::make_pair(from, 0));

	struct node *min_node = get_min_distance_node_from_unselected_nodes(distance_map, selected_nodes);

	while (min_node != NULL) {
		/*  原始点到 min_node (跳转点) 最小距离 distance */
		int distance = distance_map.find(min_node)->second;

	    for (auto e : min_node->edges) {
			struct node *n_to = e->to;
			if (!distance_map.count(n_to)) {
				distance_map.insert(std::make_pair(n_to, distance + e->weight));
			} else {
				distance_map[n_to] = std::min(distance_map[n_to], distance + e->weight);
			}
		}

		selected_nodes.insert(min_node);

		min_node = get_min_distance_node_from_unselected_nodes(distance_map, selected_nodes);
	}

	printf("dijkstra:\n");
    for (auto it : distance_map) {
		struct node *node = it.first;
		int dist = it.second;

		printf(" %2d -> %2d:  %2d\n", from->val, node->val, dist);
	}
	printf("\n");

	return distance_map;
}

/*---8>-----------------------------------------------------------------------*/

#define HEAP_NODES_MAX      256

struct node_record {
	struct node *node;
	int distance;
};

struct heap_greater {
	int h_size;
	struct node *nodes[HEAP_NODES_MAX];

	std::map < struct node *, int > index_map;	/* key 某一个 node, value 上面堆中的位置 */
	std::map < struct node *, int > dist_map;	/* key 某一个节点, value 从源节点出发到该节点的目前最小距离 */
};

bool empty(struct heap_greater *h)
{
	return h->h_size == 0;
}

bool entered(struct heap_greater * h, struct node * node)
{
	return (h->index_map.find(node) != h->index_map.end());
}

bool in_heap(struct heap_greater * h, struct node * node)
{
	return entered(h, node) && h->index_map[node] != -1;
}

void swap(struct heap_greater *h, int i, int j)
{
	h->index_map[h->nodes[i]] = j;
	h->index_map[h->nodes[j]] = i;

	struct node *tmp = h->nodes[i];
	h->nodes[i] = h->nodes[j];
	h->nodes[j] = tmp;
}

void heap_insert(struct heap_greater *h, int idx)
{
	while (h->dist_map[h->nodes[idx]] < h->dist_map[h->nodes[(idx - 1) / 2]]) {
		swap(h, idx, (idx - 1) / 2);
		idx = (idx - 1) / 2;
	}
}

void heapify(struct heap_greater *h, int idx)
{
	int left = idx * 2 + 1;

	while (left < h->h_size) {
		// ? ?
		int least = left + 1 < h->h_size && h->dist_map[h->nodes[left + 1]] < h->dist_map[h->nodes[left]]
		    ? left + 1 : left;

		least = h->dist_map[h->nodes[least]] < h->dist_map[h->nodes[idx]] ? least : idx;
		if (least == idx) {
			break;
		}

		swap(h, least, idx);
		idx = least;
		left = idx * 2 + 1;
	}
}

void heap_greater_init(struct heap_greater *h)
{
	h->h_size = 0;
	h->index_map.clear();
	h->dist_map.clear();

	for (int i = 0; i < HEAP_NODES_MAX; i++) {
		h->nodes[i] = NULL;
	}
}

/*
 * 有一个点叫 node, 现在发现了一个从源节点出发到达 node 的距离为 distance
 * 判断要不要更新, 如果需要的话, 就更新
 * 已弹出的 node ( idx: -1 ), ignore
 */
void add_or_update_or_ignore(struct heap_greater *h, struct node *node, int distance)
{
	if (h->h_size >= HEAP_NODES_MAX) {
		printf("heap full!\n");
		return;
	}

	if (in_heap(h, node)) {
		h->dist_map[node] = std::min(h->dist_map[node], distance);

		heap_insert(h, h->index_map[node]);
	}

	if (!entered(h, node)) {
		h->nodes[h->h_size] = node;
		h->index_map.insert(std::make_pair(node, h->h_size));
		h->dist_map.insert(std::make_pair(node, distance));

		heap_insert(h, h->h_size++);
	}

	/* TODO: ignore */
}

struct node_record pop(struct heap_greater *h)
{
	struct node_record record = {
		.node = h->nodes[0],
		.distance = h->dist_map[h->nodes[0]],
	};

	swap(h, 0, h->h_size - 1);

	h->index_map[h->nodes[h->h_size - 1]] = -1;
	h->dist_map.erase(h->nodes[h->h_size - 1]);
	h->nodes[h->h_size - 1] = NULL;

	h->h_size--;
	heapify(h, 0);

	return record;
}

std::map < struct node *, int > improveled_dijkstra(struct node *from)
{
	std::map < struct node *, int > result;

	struct heap_greater h;
	heap_greater_init(&h);

	add_or_update_or_ignore(&h, from, 0);

	while (!empty(&h)) {
		struct node_record record = pop(&h);

		struct node *cur = record.node;
		int dist = record.distance;

	    for (auto e : cur->edges) {
			add_or_update_or_ignore(&h, e->to, e->weight + dist);
		}

		result.insert(std::make_pair(cur, dist));
	}

	printf("improveled dijkstra:\n");
    for (auto it : result) {
		struct node *node = it.first;
		int dist = it.second;

		printf(" %2d -> %2d:  %2d\n", from->val, node->val, dist);
	}
	printf("\n");

	return result;
}

/*---8<-----------------------------------------------------------------------*/

/*
 *              4
 *         ^    ^   \
 *        /     |    v
 *        1 —>  3  —> 5
 *        \     ^     ^
 *         v    |    /
 *              2
 */
int main(int argc, char **argv)
{
	int matrix[][COLS] = {
		{ 1, 1, 2 },
		{ 7, 1, 3 },
		{ 6, 1, 4 },
		{ 2, 2, 3 },
		{ 55, 2, 5 },
		{ 2, 3, 4 },
		{ 23, 3, 5 },
		{ 4, 4, 5 },
	};

	struct graph graph = create_entire_graph(matrix, ARRAY_SIZE(matrix), COLS);

	print_entire_graph(&graph);

	struct node *start = graph.nodes[1];

	std::map < struct node *, int > map = dijkstra(start);

	std::map < struct node *, int > imp_map = improveled_dijkstra(start);

	destroy_entire_graph(&graph);

	return 0;
}
