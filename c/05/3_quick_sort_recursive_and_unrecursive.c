
#include "util.h"

#include <stack>
#include <queue>

/*
 * arr[L...R] 玩荷兰国旗问题的划分, 以arr[R]做划分值
 * < arr[R] ~~ == arr[R] ~~ > arr[R]
 */
void netherlands_flag(int arr[], int l, int r, int m_equal_area[])
{
	if (l > r) {
		m_equal_area[0] = -1;
		m_equal_area[1] = -1;
	}

	if (l == r) {
		m_equal_area[0] = l;
		m_equal_area[1] = l;
	}

	int less = l - 1;	    /* < 区右边界 */
	int more = r;		    /* > 区左边界 */
	int index = l;
	while (index < more) {
		if (arr[index] == arr[r]) {
			index++;
		} else if (arr[index] < arr[r]) {
			swap(arr, index++, ++less);
		} else {
			swap(arr, index, --more);
		}
	}

	swap(arr, more, r);

	m_equal_area[0] = less + 1;
	m_equal_area[1] = more;
}

void process(int arr[], int l, int r)
{
	if (l >= r) {		/* base case */
		return;
	}

	swap(arr, l + (int)(RANDOM() * (r - l + 1)), r);

	int m_equal_area[2] = { -1, -1 };

	netherlands_flag(arr, l, r, m_equal_area);

	process(arr, l, m_equal_area[0] - 1);
	process(arr, m_equal_area[1] + 1, r);
}

/* 快排递归版本 */
void quick_sort(int arr[], int n)
{
	if (arr == NULL || n < 2) {
		return;
	}

	process(arr, 0, n - 1);
}

/*
 * 快排非递归版本需要的辅助类
 * 要处理的是什么范围上的排序
 */
struct op {
	int left;
	int right;
};

/* 快排3.0 非递归版本 用栈来执行 */
void quick_sort_un_with_stack(int arr[], int n)
{
	if (arr == NULL || n < 2) {
		return;
	}

	swap(arr, (int)(RANDOM() * n), n - 1);

	int m_equal_area[2] = { -1, -1 };

	netherlands_flag(arr, 0, n - 1, m_equal_area);

	std::stack < struct op >s;
	struct op op;

	op.left = 0;
	op.right = m_equal_area[0] - 1;
	s.push(op);

	op.left = m_equal_area[1] + 1;
	op.right = n - 1;
	s.push(op);

	while (!s.empty()) {
		op = s.top();	/* op.l ... op.r */
		s.pop();

		int opl = op.left, opr = op.right;

		if (opl < opr) {
			swap(arr, opl + (int)(RANDOM() * (opr - opl + 1)), opr);

			netherlands_flag(arr, opl, opr, m_equal_area);

			op.left = opl;
			op.right = m_equal_area[0] - 1;
			s.push(op);

			op.left = m_equal_area[1] + 1;
			op.right = opr;
			s.push(op);
		}
	}
}

int main(int argc, char *argv[])
{
	int max_size = 15;
	int max_value = 20;
	int times = 10;
	bool succeed = true;

	srand((int)time(0));

	for (int i = 0; i < times; i++) {
		int arr1[max_size];
		int arr2[max_size];
		int actual_size = 0;

		create_random_array(max_size, max_value, arr1, &actual_size);
		memcpy(arr2, arr1, actual_size * sizeof(int));

		quick_sort(arr1, actual_size);

		printf("[%d]:\t", actual_size);
		print_array_data(arr2, actual_size);
		printf("sort:\t");
		print_array_data(arr1, actual_size);
		printf("---8<---------------------------------------------------\n\n");

		quick_sort_un_with_stack(arr2, actual_size);
		//qsort(arr2, actual_size, sizeof(int), cmp);

		if (!is_equal(arr1, arr2, actual_size)) {
			succeed = false;
			break;
		}
	}

	printf("result: %s\n", succeed ? "nice" : "fucked");

	return 0;
}
