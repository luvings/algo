
#include <limits.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <queue>
#include <string>

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

struct info {
	bool b_full;
	bool b_cbt;
	int height;
};

/* 对每一棵子树, 是否是满二叉树、是否是完全二叉树、高度 */
struct info process(struct tree_desc *x)
{
	struct info rep = { true, true, 0, };
	if (x == NULL) {
		return rep;
	}

	struct info l_info = process(x->left);
	struct info r_info = process(x->right);

	int height = std::max(l_info.height, r_info.height) + 1;

	bool b_full = l_info.b_full && r_info.b_full && l_info.height == r_info.height;

	bool b_cbt = false;
	if (b_full) {
		b_cbt = true;
	} else {
		/* 以 x 为头整棵树, 不满 */
		if (l_info.b_cbt && r_info.b_cbt) {
			if (l_info.b_cbt && r_info.b_full && l_info.height == r_info.height + 1) {
				b_cbt = true;
			}

			if (l_info.b_full && r_info.b_full && l_info.height == r_info.height + 1) {
				b_cbt = true;
			}

			if (l_info.b_full && r_info.b_cbt && l_info.height == r_info.height) {
				b_cbt = true;
			}
		}
	}

	rep.b_full = b_full;
	rep.b_cbt = b_cbt;
	rep.height = height;

	return rep;
}

bool is_complete_binay_tree(struct tree_desc * head)
{
	if (head == NULL) {
		return true;
	}

	return process(head).b_cbt;
}

bool is_complete_binay_tree_cmp(struct tree_desc * head)
{
	if (head == NULL) {
		return true;
	}

	std::queue < struct tree_desc * > q;
	bool leaf = false;      /* 是否遇到过左右两个孩子不双全的节点 */
	q.push(head);

	struct tree_desc *l = NULL;
	struct tree_desc *r = NULL;

	while (!q.empty()) {
		head = q.front();
		q.pop();

		l = head->left;
		r = head->right;

		if ((l == NULL && r != NULL) ||
		    /* 如果遇到了不双全的节点之后, 又发现当前节点不是叶节点 */
		    (leaf && (l != NULL || r != NULL))
		    ) {
			return false;
		}

		if (l != NULL) {
			q.push(l);
		}
		if (r != NULL) {
			q.push(r);
		}

		if (l == NULL || r == NULL) {
			leaf = true;
		}
	}

	return true;
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

std::string space(int n)
{
	std::string s;

	s.resize(n, ' ');

	return s;
}

void in_order_p(struct tree_desc *head, int height, std::string to, int width)
{
	if (head == NULL) {
		return;
	}

	in_order_p(head->right, height + 1, "v", width);

	std::string val = to + std::to_string(head->val) + to;
	int len_m = val.length();
	int len_l = (width - len_m) >> 1;
	int len_r = width - len_m - len_l;
	val = space(len_l) + val + space(len_r);
	printf("%s%s\n", space(height * width).c_str(), val.c_str());

	in_order_p(head->left, height + 1, "^", width);
}

void print_tree_table(struct tree_desc *head)
{
	printf("\n\n");

	in_order_p(head, 0, "H", 12);

	printf("\n\n\n\n");
}

int main(int argc, char **argv)
{
	int max_level = 5;
	int max_value = 100;
	int times = 5;
	bool succeed = true;

	srandom((unsigned) time(0));

	for (int i = 0; i < times; i++) {
		struct tree_desc *head = create_random_tree_table(max_level, max_value);
		//print_tree_table(head);

		if (is_complete_binay_tree(head) != is_complete_binay_tree_cmp(head)) {
			printf("oops\n");
			succeed = false;
			break;
		}

		delete_tree_table(head);
	}

	printf("result: %s\n", succeed ? "nice" : "fucked");

	return 0;
}
