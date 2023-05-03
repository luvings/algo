/*
 * 二叉树可以通过先序、后序或者按层遍历的方式序列化和反序列化,
 * 以下代码全部实现了
 * 但是, 二叉树无法通过中序遍历的方式实现序列化和反序列化
 * 因为不同的两棵树, 可能得到同样的中序序列, 即便补了空位置也可能一样
 * 比如如下两棵树
 *         __2
 *        /
 *       1
 *       和
 *       1__
 *          \
 *           2
 * 补足空位置的中序遍历结果都是 { #, 1, #, 2, # }
 *
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <queue>
#include <stack>
#include <string>
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

void prev_order_s(struct tree_desc *head, std::queue < std::string > &q)
{
	if (head == NULL) {
		q.push("#");
	} else {
		//printf("%d, ", head->val);
		q.push(std::to_string(head->val));
		prev_order_s(head->left, q);
		prev_order_s(head->right, q);
	}
}

std::queue < std::string > prev_serialize(struct tree_desc *head)
{
	std::queue < std::string > q;

	prev_order_s(head, q);

	return q;
}

void post_order_s(struct tree_desc *head, std::queue < std::string > &q)
{
	if (head == NULL) {
		q.push("#");
	} else {
		post_order_s(head->left, q);
		post_order_s(head->right, q);
		//printf("%d, ", head->val);
		q.push(std::to_string(head->val));
	}
}

std::queue < std::string > post_serialize(struct tree_desc *head)
{
	std::queue < std::string > q;

	post_order_s(head, q);

	return q;
}

std::queue < std::string > level_serialize(struct tree_desc * head)
{
	std::queue < std::string > s_q;

	if (head == NULL) {
		s_q.push("#");
	} else {
		s_q.push(std::to_string(head->val));

		std::queue < struct tree_desc * > q;
		q.push(head);

		while (!q.empty()) {
			head = q.front();
			q.pop();

			if (head->left != NULL) {
				q.push(head->left);
				s_q.push(std::to_string(head->left->val));
			} else {
				s_q.push("#");
			}

			if (head->right != NULL) {
				q.push(head->right);
				s_q.push(std::to_string(head->right->val));
			} else {
				s_q.push("#");
			}
		}
	}

	return s_q;
}

struct tree_desc *prev_order_b(std::queue < std::string > &q)
{
	std::string s = q.front();
	q.pop();
	if (s == "#") {
		return NULL;
	}

	struct tree_desc *head = NEW_NODE(struct tree_desc, atoi(s.c_str()));
	head->left = prev_order_b(q);
	head->right = prev_order_b(q);

	return head;
}

struct tree_desc *build_tree_by_prev_q(std::queue < std::string > q)
{
	if (q.size() == 0) {
		return NULL;
	}

	return prev_order_b(q);
}

struct tree_desc *post_order_b(std::stack < std::string > &stk)
{
	std::string s = stk.top();
	stk.pop();
	if (s == "#") {
		return NULL;
	}

	struct tree_desc *head = NEW_NODE(struct tree_desc, atoi(s.c_str()));
	head->right = post_order_b(stk);
	head->left = post_order_b(stk);

	return head;
}

struct tree_desc *build_tree_by_post_q(std::queue < std::string > q)
{
	if (q.size() == 0) {
		return NULL;
	}

	/* 左右中  ->  stack(中右左) */
	std::stack < std::string > stk;
	while (!q.empty()) {
		std::string s = q.front();
		q.pop();

		stk.push(s);
	}

	return post_order_b(stk);
}

struct tree_desc *build_tree_by_level_q(std::queue < std::string > level_s)
{
	if (level_s.size() == 0 || level_s.front() == "#") {
		return NULL;
	}

	struct tree_desc *head = NEW_NODE(struct tree_desc, atoi(level_s.front().c_str()));
	level_s.pop();

	std::queue < struct tree_desc * > q;

	if (head != NULL && level_s.size() > 0) {
		q.push(head);
	}

	while (!q.empty()) {
		struct tree_desc *cur = q.front();
		q.pop();

		cur->left = (level_s.front() == "#") ? NULL : NEW_NODE(struct tree_desc, atoi(level_s.front().c_str()));
		level_s.pop();

		cur->right =
		    (level_s.front() == "#") ? NULL : NEW_NODE(struct tree_desc, atoi(level_s.front().c_str()));
		level_s.pop();

		if (cur->left != NULL) {
			q.push(cur->left);
		}
		if (cur->right != NULL) {
			q.push(cur->right);
		}
	}

	return head;
}

bool is_same_btree(struct tree_desc * head1, struct tree_desc * head2)
{
	if (head1 == NULL && head2 != NULL) {
		return false;
	}
	if (head1 != NULL && head2 == NULL) {
		return false;
	}
	if (head1 == NULL && head2 == NULL) {
		return true;
	}
	if (head1->val != head2->val) {
		return false;
	} else {
		return true;
	}

	return is_same_btree(head1->left, head2->left) && is_same_btree(head1->right, head2->right);
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
	int max_level = 5;
	int max_value = 100;
	int times = 5;
	bool succeed = true;

	srandom((unsigned) time(0));

	for (int i = 0; i < times; i++) {
		struct tree_desc *head = create_random_tree_table(max_level, max_value);

		std::queue < std::string > prev_s = prev_serialize(head);
		std::queue < std::string > post_s = post_serialize(head);
		std::queue < std::string > level_s = level_serialize(head);

		printf("(%2ld):\t", level_s.size());
		std::queue < std::string > q = level_s;
		while (!q.empty()) {
			printf("%s, ", q.front().c_str());
			q.pop();
		}
		printf("\n");

		struct tree_desc *prev_h = build_tree_by_prev_q(prev_s);
		struct tree_desc *post_h = build_tree_by_post_q(post_s);
		struct tree_desc *level_h = build_tree_by_level_q(level_s);

		if (!is_same_btree(prev_h, post_h) || !is_same_btree(prev_h, level_h)) {
			printf("oops\n");
			succeed = false;
			break;
		}

		delete_tree_table(head);
	}

	printf("result: %s\n", succeed ? "nice" : "fucked");

	return 0;
}
