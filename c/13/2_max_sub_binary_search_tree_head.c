
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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
	struct tree_desc *max_sub_bst_h;
	int max_sub_bst_size;
	int max;
	int min;
};

struct info process(struct tree_desc *x)
{
	struct info rep = { true, NULL, 0, };
	if (x == NULL) {
		return rep;
	}

	struct info l_info = process(x->left);
	struct info r_info = process(x->right);

	int min = x->val, max = x->val;

	struct tree_desc *max_sub_bst_h = NULL;
	int max_sub_bst_size = 0;

	if (!l_info.b_null) {
		max = std::max(l_info.max, max);
		min = std::min(l_info.min, min);
		max_sub_bst_h = l_info.max_sub_bst_h;
		max_sub_bst_size = l_info.max_sub_bst_size;
	}

	if (!r_info.b_null) {
		max = std::max(r_info.max, max);
		min = std::min(r_info.min, min);
		if (r_info.max_sub_bst_size > max_sub_bst_size) {
			max_sub_bst_h = r_info.max_sub_bst_h;
			max_sub_bst_size = r_info.max_sub_bst_size;
		}
	}

	if ((l_info.b_null ? true : (l_info.max_sub_bst_h == x->left && l_info.max < x->val)) &&
	    (r_info.b_null ? true : (r_info.max_sub_bst_h == x->right && r_info.min > x->val))
	    ) {
		max_sub_bst_h = x;
		max_sub_bst_size = (l_info.b_null ? 0 : l_info.max_sub_bst_size)
		    + (r_info.b_null ? 0 : r_info.max_sub_bst_size) + 1;
	}

	rep.b_null = false;
	rep.max_sub_bst_h = max_sub_bst_h;
	rep.max_sub_bst_size = max_sub_bst_size;
	rep.max = max;
	rep.min = min;

	return rep;
}

struct tree_desc *max_sub_bst_head(struct tree_desc *head)
{
	if (head == NULL) {
		return NULL;
	}

	return process(head).max_sub_bst_h;
}

void in(struct tree_desc *head, std::vector < struct tree_desc * > &v)
{
	if (head == NULL) {
		return;
	}

	in(head->left, v);
	v.push_back(head);
	in(head->right, v);
}

int get_bst_size(struct tree_desc *head)
{
	if (head == NULL) {
		return 0;
	}

	std::vector < struct tree_desc * > v;
	in(head, v);

	for (uint32_t i = 1; i < v.size(); i++) {
		if (v.at(i)->val <= v.at(i - 1)->val) {
			return 0;
		}
	}

	return v.size();
}

struct tree_desc *max_sub_bst_head_cmp(struct tree_desc *head)
{
	if (head == NULL) {
		return NULL;
	}

	if (get_bst_size(head) != 0) {
		return head;
	}

	struct tree_desc *bst_left_h = max_sub_bst_head_cmp(head->left);
	struct tree_desc *bst_right_h = max_sub_bst_head_cmp(head->right);

	return get_bst_size(bst_left_h) >= get_bst_size(bst_right_h) ? bst_left_h : bst_right_h;
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
	int max_level = 6;
	int max_value = 100;
	int times = 5;
	bool succeed = true;

	srandom((unsigned) time(0));

	for (int i = 0; i < times; i++) {
		struct tree_desc *head = create_random_tree_table(max_level, max_value);
		print_tree_table(head);

		struct tree_desc *max_sub_bst_h = max_sub_bst_head(head);
		printf("max sub BST head->val: %d\n", max_sub_bst_h ? max_sub_bst_h->val : -111);

		if (max_sub_bst_head(head) != max_sub_bst_head_cmp(head)) {
			print_tree_table(head);
			printf("oops\n");
			succeed = false;
			break;
		}

		delete_tree_table(head);
	}

	printf("result: %s\n", succeed ? "nice" : "fucked");

	return 0;
}
