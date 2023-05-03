
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

/* arr中, 只有一种数, 出现奇数次 */
void print_odd_times_num1(int arr[], int n)
{
	int eor = 0;

	for (int i = 0; i < n; i++) {
		eor ^= arr[i];
	}

	printf("eor = %d\n", eor);
}

/* arr中, 有两种数, 出现奇数次 */
void print_odd_times_num2(int arr[], int n)
{
	int eor = 0;

	for (int i = 0; i < n; i++) {
		eor ^= arr[i];
	}

	/*
	 * a 和 b是两种数
	 * eor != 0
	 * eor最右侧的1, 提取出来
	 * eor :     00110010110111000
	 * -eor :    11001101001000111 | 1
	 * right_1 : 00000000000001000
	 */
	int right_1 = eor & (-eor);	/* 提取出最右的 1 */

	int only_1 = 0;		/* eor' */

	for (int i = 0; i < n; i++) {
		/*
		 *  arr[1] =  111100011110000
		 * right_1 =  000000000010000
		 */
		if ((arr[i] & right_1) != 0) {
			only_1 ^= arr[i];
		}
	}

	printf("only_1 = %d, (eor ^ only_1) = %d\n", only_1, (eor ^ only_1));
}

int bit1_counts(int num)
{
	int count = 0;

	/*
	 *   011011010000
	 *   000000010000
	 *
	 *   011011000000
	 */
	while (num != 0) {
		int right_1 = num & ((~num) + 1);
		count++;
		num ^= right_1;
		// num -= right_1
	}

	printf("bit 1 count = %d\n", count);

	return count;
}

int main(int argc, char *argv[])
{
	int arr1[] = { 3, 2, 3, 1, 1, };
	print_odd_times_num1(arr1, ARRAY_SIZE(arr1));

	int arr2[] = { 4, 3, 4, 2, 2, 2, 3, 1, };
	print_odd_times_num2(arr2, ARRAY_SIZE(arr2));

	int num = 0xFE;
	bit1_counts(num);

	return 0;
}
