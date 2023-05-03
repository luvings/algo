
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <map>
#include <queue>
#include <vector>

/* [0, 1) */
#define RANDOM()    (random() / (RAND_MAX + 1.0))

#define NEW_NODE(type, var)   ({                    \
    type *__ptr = (type *) malloc(sizeof(type));    \
    assert(__ptr);                                  \
    memset((void *)__ptr, 0, sizeof(type));         \
    __ptr->val   = (var);                           \
    __ptr->left  = NULL;                            \
    __ptr->right = NULL;                            \
    __ptr;                                          \
})

struct tree_desc {
	int val;

	struct tree_desc *left;
	struct tree_desc *right;
};

int btree_max_width_with_map(struct tree_desc *head)
{
	if (head == NULL) {
		return 0;
	}

	std::queue < struct tree_desc * > q;
	q.push(head);

	std::map < struct tree_desc *, int > map;
	map.insert(std::make_pair(head, 1));

	int current_level = 1;	        /* 当前你正在统计哪一层的宽度 */
	int current_level_nodes = 0;	/* 当前层curLevel层, 宽度目前是多少 */
	int max_nodes = 0;

	while (!q.empty()) {
		struct tree_desc *cur = q.front();
		q.pop();

		int cur_node_level = map.find(cur)->second;

		if (cur->left != NULL) {
			q.push(cur->left);
			map.insert(std::make_pair(cur->left, cur_node_level + 1));
		}
		if (cur->right != NULL) {
			q.push(cur->right);
			map.insert(std::make_pair(cur->right, cur_node_level + 1));
		}

		if (cur_node_level == current_level) {
			current_level_nodes++;
		} else {
			/* pop 新一层节点时, 统计上一层节点数 */
			max_nodes = std::max(max_nodes, current_level_nodes);
			current_level++;
			current_level_nodes = 1;
		}
	}

	max_nodes = std::max(max_nodes, current_level_nodes);

	return max_nodes;
}

int btree_max_width(struct tree_desc *head)
{
	if (head == NULL) {
		return 0;
	}

	std::queue < struct tree_desc * > q;
	q.push(head);

	struct tree_desc *current_end = head;
	struct tree_desc *next_end = NULL;

	int current_level_nodes = 0;
	int max_nodes = 0;

	while (!q.empty()) {
		struct tree_desc *cur = q.front();
		q.pop();

		if (cur->left != NULL) {
			q.push(cur->left);
			next_end = cur->left;
		}
		if (cur->right != NULL) {
			q.push(cur->right);
			next_end = cur->right;
		}

		current_level_nodes++;

		if (cur == current_end) {
			max_nodes = std::max(max_nodes, current_level_nodes);
			current_level_nodes = 0;
			current_end = next_end;
		}
	}

	return max_nodes;
}

struct tree_desc *alloc_tree_node(int level, int max_level, int max_value)
{
	if (level > max_level || RANDOM() < 0.5) {
		return NULL;
	}

	struct tree_desc *head = NEW_NODE(struct tree_desc, (int)(RANDOM() * (max_value + 1)));
	head->left = alloc_tree_node(level + 1, max_level, max_value);
	head->right = alloc_tree_node(level + 1, max_level, max_value);

	return head;
}

struct tree_desc *create_random_tree_table(int max_level, int max_value)
{
	return alloc_tree_node(1, max_level, max_value);
}

void delete_tree_table(struct tree_desc *head)
{
	if (!head) {
		return;
	}

	delete_tree_table(head->left);
	delete_tree_table(head->right);
	free(head);
}

int main(int argc, char **argv)
{
	int max_level = 10;
	int max_value = 100;
	int times = 5;
	bool succeed = true;

	srandom((unsigned) time(0));

	for (int i = 0; i < times; i++) {
		struct tree_desc *head = create_random_tree_table(max_level, max_value);

		if (btree_max_width(head) != btree_max_width_with_map(head)) {
			printf("oops\n");
			succeed = false;
			break;
		}

		delete_tree_table(head);
	}

	printf("result: %s\n", succeed ? "nice" : "fucked");

	return 0;
}
