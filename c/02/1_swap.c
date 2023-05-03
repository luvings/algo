
#include <stdio.h>

/*
 *  ^ 异或运算 -> " 无进位相加 ", 满足交换律和结合律
 *
 *  0 ^ N = N
 *
 *  N ^ N = 0
 *
 *  a ^ b ^ a = (a ^ a) ^ b = 0 ^ b = b
 */
static void swap(int *data, int i, int j)
{
	/*
	 * Attn: i cannot be equal to j
	 *
	 * data[0] = data[0] ^ data[0]
	 */
	data[i] = data[i] ^ data[j];
	data[j] = data[i] ^ data[j];
	data[i] = data[i] ^ data[j];
}

int main(int argc, char *argv[])
{
	int a = 16;
	int b = 603;

	printf("a = %d\n", a);
	printf("b = %d\n", b);

	a = a ^ b;
	b = a ^ b;
	a = a ^ b;

	printf("a = %d\n", a);
	printf("b = %d\n", b);

	printf("\n\n");

	int arr[] = { 3, 1, 100 };
	int i = 0;
	int j = 0;

	arr[i] = arr[i] ^ arr[j];
	arr[j] = arr[i] ^ arr[j];
	arr[i] = arr[i] ^ arr[j];

	printf("arr[i] = %d, arr[j] = %d\n", arr[i], arr[j]);

	printf("\nidx cann't be the same when swapped by '^'\n");

	printf("\n\n");

	arr[0] = 3;
	arr[2] = 5;

	printf("arr[0] = %d, arr[2] = %d\n", arr[i], arr[j]);

	swap(arr, 0, 2);

	printf("arr[0] = %d, arr[2] = %d\n", arr[i], arr[j]);

	return 0;
}
