
#include <limits.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <stack>
#include <vector>
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
	bool b_null;
	bool b_bst;
	int max;
	int min;
};

struct info process(struct tree_desc *x)
{
	struct info rep = { true, true, 0, };
	if (x == NULL) {
		return rep;
	}

	struct info l_info = process(x->left);
	struct info r_info = process(x->right);

	int max = x->val, min = x->val;
	if (!l_info.b_null) {
		max = std::max(l_info.max, max);
		min = std::min(l_info.min, min);
	}
	if (!r_info.b_null) {
		max = std::max(r_info.max, max);
		min = std::min(r_info.min, min);
	}

	bool b_bst = true;
	if (!l_info.b_null && (!l_info.b_bst || l_info.max >= x->val)) {
		b_bst = false;
	}
	if (!r_info.b_null && (!r_info.b_bst || r_info.min <= x->val)) {
		b_bst = false;
	}

/*
	bool b_bst = false;
	if (
        (!l_info.b_null ? (l_info.b_bst && l_info.max < x->val) : true)
        && (!r_info.b_null ? (r_info.b_bst && r_info.min > x->val) : true)
	    ) {
		b_bst = true;
	}
*/

	rep.b_null = false;
	rep.b_bst = b_bst;
	rep.max = max;
	rep.min = min;

	return rep;
}

bool is_binay_search_tree(struct tree_desc * head)
{
	return process(head).b_bst;
}

void in(struct tree_desc *head, std::vector < struct tree_desc * > &tree_list)
{
	if (head == NULL) {
		return;
	}

	in(head->left, tree_list);
	tree_list.push_back(head);
	in(head->right, tree_list);
}

bool is_binay_search_tree_cmp(struct tree_desc * head)
{
	if (head == NULL) {
		return true;
	}

	std::vector < struct tree_desc * > tree_list;
	in(head, tree_list);

	for (uint32_t i = 1; i < tree_list.size(); i++) {
		if (tree_list.at(i)->val <= tree_list.at(i - 1)->val) {
			return false;
		}
	}

	return true;
}

/*---8>-----------------------------------------------------------------------*/

/*
 *           5
 *         /   \
 *        3     7
 *       / \   / \
 *      2   4 6   8
 *     /
 *    1
 *
 * in order: 1, 2, 3, 4, 5, 6, 7, 8
 */

static int s_prev_value = INT_MIN;

bool check_bst_with_in_order_recur(struct tree_desc *head, int depth)
{
	if (depth == 1) {
		s_prev_value = INT_MIN;
	}

	if (head == NULL) {
		return true;
	}

	bool b_left_bst = check_bst_with_in_order_recur(head->left, depth + 1);
	if (!b_left_bst) {
		return false;
	}

	if (s_prev_value >= head->val) {
		return false;
	} else {
		s_prev_value = head->val;
	}

	return check_bst_with_in_order_recur(head->right, depth + 1);
}

bool check_bst_with_in_order_un_recur(struct tree_desc * head)
{
	if (head != NULL) {
		struct tree_desc *cur = head;
		std::stack < struct tree_desc * > s;
		// ?
		int prev_value = INT_MIN;

		while (!s.empty() || cur != NULL) {
			if (cur != NULL) {
				s.push(cur);
				cur = cur->left;
			} else {
				cur = s.top();
				s.pop();
				if (prev_value >= cur->val) {
					return false;
				} else {
					prev_value = cur->val;
				}
				cur = cur->right;
			}
		}
	}

	return true;
}

/*---8<-----------------------------------------------------------------------*/

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

		if (is_binay_search_tree(head) != is_binay_search_tree_cmp(head)) {
			printf("oops..1\n");
			succeed = false;
			break;
		}

		if (check_bst_with_in_order_recur(head, 1) != check_bst_with_in_order_un_recur(head)) {
			printf("oops..2\n");
			succeed = false;
			break;
		}

		delete_tree_table(head);
	}

	printf("result: %s\n", succeed ? "nice" : "fucked");

	return 0;
}
