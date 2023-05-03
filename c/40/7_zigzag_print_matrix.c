
#include <stdio.h>
#include <stdbool.h>

#define ROWS            3
#define COLS            4

#define ARRAY_SIZE(x)   (sizeof(x) / sizeof((x)[0]))

void print_diagonal_line(int matrix[][COLS], int ar, int ac, int br, int bc, bool fromup)
{
	if (fromup) {
		while (br != ar - 1) {
			printf("%4d ", matrix[br--][bc++]);
		}
	} else {
		while (ar != br + 1) {
			printf("%4d ", matrix[ar++][ac--]);
		}
	}
}

/*
 * rows -> 行, 排
 * cols -> 列
 *
 *  o  o  o  o
 *  o  o  o  o
 *  o  o  o  o
 *
 */
void zigzag_print_matrix(int matrix[][COLS], int rows, int cols)
{
	int ar = 0, ac = 0;
	int br = 0, bc = 0;
	int er = rows - 1, ec = cols - 1;

	bool fromup = true;

	while (ar != er + 1) {
		print_diagonal_line(matrix, ar, ac, br, bc, fromup);

		/* A 点右移一列, B 点下移一行 */
		ar = (ac == ec) ? ar + 1 : ar;
		ac = (ac == ec) ? ac : ac + 1;

		bc = (br == er) ? bc + 1 : bc;
		br = (br == er) ? br : br + 1;

		fromup = !fromup;
	}
	printf("\n");
}

int main(int argc, char **argv)
{
	int matrix[][COLS] = { {1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12} };

	int rows = sizeof(matrix) / sizeof(matrix[0]);
	int cols = sizeof(matrix[0]) / sizeof(matrix[0][0]);
	printf("rows = %d, cols = %d\n", rows, cols);

	zigzag_print_matrix(matrix, rows, cols);

	return 0;
}
