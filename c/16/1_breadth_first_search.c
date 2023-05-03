
#include "graph.h"

void bfs(struct node *start)
{
	printf("bfs:\t");

	if (start == NULL) {
		return;
	}

	std::queue < struct node * > q;
	std::set < struct node * > set;

	q.push(start);
	set.insert(start);

	while (!q.empty()) {
		struct node *cur = q.front();
		q.pop();
		printf("%2d,  ", cur->val);

	    for (auto next : cur->nexts) {
			if (!set.count(next)) {
				q.push(next);
				set.insert(next);
			}
		}
	}
	printf("\n");
}

/*
 * stack 栈底 --> 栈顶顺序, 即为点经过的路径
 */
void dfs(struct node *start)
{
	printf("dfs:\t");

	if (start == NULL) {
		return;
	}

	std::stack < struct node * > s;
	std::set < struct node * > set;

	s.push(start);
	set.insert(start);
	printf("%2d,  ", start->val);

	while (!s.empty()) {
		struct node *cur = s.top();
		s.pop();

	    for (auto next : cur->nexts) {
			if (!set.count(next)) {
				s.push(cur);
				s.push(next);
				set.insert(next);
				printf("%2d,  ", next->val);
				break;
			}
		}
	}
	printf("\n");
}

/*
 *            1
 *         /  |  \
 *        v   |   \
 *       2 —— 3 —> 4
 *        \   |   ^
 *         v  v  /
 *            5
 */
int main(int argc, char **argv)
{
	int matrix[][COLS] = {
		{ 1, 1, 2 },
		{ 1, 2, 5 },
		{ 1, 3, 1 },
		{ 1, 3, 2 },
		{ 1, 3, 4 },
		{ 1, 1, 3 },
		{ 1, 1, 4 },
		{ 1, 2, 3 },
		{ 1, 3, 5 },
		{ 1, 4, 1 },
		{ 1, 5, 4 },
	};

	struct graph graph = create_entire_graph(matrix, ARRAY_SIZE(matrix), COLS);

	print_entire_graph(&graph);

	struct node *start = graph.nodes[1];
	bfs(start);
	dfs(start);

	destroy_entire_graph(&graph);

	return 0;
}
