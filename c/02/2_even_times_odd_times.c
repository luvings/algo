
#include <stdio.h>

/*
 * 位域不能为负数, 否则编译时报错
 *
 * 数组名 a 用作函数参数时, 因数组名代表数组首元素的起始地址, a 会退化为指针
 *
 * __builtin_types_compatible_p 是 gcc 内联函数
 *
 * 1) 若 a 是数组类型, &(a)[0] 是指针类型.      __same_type 返回 0
 * 2) 若 a 是指针类型, &(a)[0] 仍然是指针类型.  __same_type 返回 1
 */
#define BUILD_BUG_ON_ZERO(e)    (sizeof(struct { int:-!!(e); }))
#define __same_type(a, b)       __builtin_types_compatible_p(typeof(a), typeof(b))
#define __must_be_array(a)      BUILD_BUG_ON_ZERO(__same_type((a), &(a)[0]))
#define ARRAY_SIZE(arr)         (sizeof(arr) / sizeof((arr)[0]) + __must_be_array(arr))

/* arr[] 只有一种数, 出现奇数次 */
void print_odd_times_num1(int arr[], int n)
{
	int eor = 0;

	for (int i = 0; i < n; i++) {
		eor ^= arr[i];
	}

	printf("eor = %d\n", eor);
}

/*
 * arr[] 有两种数, 出现奇数次
 *
 * 假设 a, b 为这两数, a != b, 则 eor = a ^ b, 并且 eor != 0
 *
 *
 *  n	   ( 2)  :     0010
 * ~n + 1, (-2)  :  &  1101 | 1
 * right 1       :     0010
 *
 *
 * a  ( 2)	 :     0010
 * b  (-2, -a)	 :  +  1110
 *      0	 :     0000
 * so, a, b 互为相反数
 *
 */
void print_odd_times_num2(int arr[], int n)
{
	int eor = 0;

	for (int i = 0; i < n; i++) {
		eor ^= arr[i];
	}

	/* 提取出最右的 1 */
	int right_1 = eor & (-eor);	// eor & (~eor + 1)

	int only_1 = 0;			// eor'

	for (int i = 0; i < n; i++) {
		/*
		 *  arr[1] =  1100
		 * right_1 =  0100
		 */
		if ((arr[i] & right_1) != 0) {
			only_1 ^= arr[i];
		}
	}

	printf("only_1 = %d, (eor ^ only_1) = %d\n", only_1, (eor ^ only_1));
}

int bit1_count(int num)
{
	int cnt = 0;

	/*
	 *     1101 1010
	 *   ^ 0000 0010
	 *
	 *     1101 1000
	 */
	while (num != 0) {
		int right_1 = num & (~num + 1);
		cnt++;
		num ^= right_1;
	}

	printf("bit1 count: %d\n", cnt);

	return cnt;
}

int main(int argc, char *argv[])
{
	int arr1[] = { 3, 2, 3, 1, 1, };
	print_odd_times_num1(arr1, ARRAY_SIZE(arr1));

	int arr2[] = { 4, 3, 4, 2, 2, 2, 3, 1, };
	print_odd_times_num2(arr2, ARRAY_SIZE(arr2));

	int num = 0x70;
	bit1_count(num);

	return 0;
}
