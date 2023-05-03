
#include <stdio.h>

#define ROWS            4
#define COLS            4

#define ARRAY_SIZE(x)   (sizeof(x) / sizeof((x)[0]))

/*
 * rows -> 行, 排
 * cols -> 列
 *
 * 左上角点 (a, b), 右下角点 (c, d), 确定 4 条边
 * 按照 x 标点为一组, 4 条边分 3 组, 进行旋转
 *
 *
 *        b |
 *          v
 *     a -> o  x  o  o
 *          o  o  o  x
 *          x  o  o  o
 *          o  o  x  o <- c
 *                   ^
 *                   | d
 *
 */

void rotate_4_edges(int m[][COLS], int a, int b, int c, int d)
{
	int tmp = 0;

	for (int i = 0; i < (d - b); i++) {
		tmp = m[a][b + i];
		m[a][b + i] = m[c - i][b];
		m[c - i][b] = m[c][d - i];
		m[c][d - i] = m[a + i][d];
		m[a + i][d] = tmp;
	}
}

void rotate(int matrix[][COLS], int rows, int cols)
{

	int a = 0, b = 0;
	int c = rows - 1, d = cols - 1;

	while (a < c) {
		rotate_4_edges(matrix, a++, b++, c--, d--);
	}
}

void print_matrix(int matrix[][COLS], int rows, int cols)
{
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			printf("%4d ", matrix[i][j]);
		}
		printf("\n");
	}
}

int main(int argc, char **argv)
{
	int matrix[][COLS] = { { 1, 2, 3, 4 }, { 5, 6, 7, 8 }, { 9, 10, 11, 12 }, { 13, 14, 15, 16 } };

	print_matrix(matrix, ARRAY_SIZE(matrix), COLS);

	rotate(matrix, ARRAY_SIZE(matrix), COLS);

	printf("\n------------------< rotate >-------------------------\n\n");

	print_matrix(matrix, ARRAY_SIZE(matrix), COLS);

	return 0;
}
