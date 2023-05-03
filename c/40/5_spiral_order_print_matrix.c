
#include <stdio.h>

#define ROWS            4
#define COLS            4

#define ARRAY_SIZE(x)   (sizeof(x) / sizeof((x)[0]))

void print_4_edges(int matrix[][COLS], int fr, int fc, int er, int ec)
{
	if (fr == er) {
		for (int i = fc; i <= ec; i++) {
			printf("%4d ", matrix[fr][i]);
		}
	} else if (fc == ec) {
		for (int i = fr; i <= er; i++) {
			printf("%4d ", matrix[i][fc]);
		}
	} else {
		int rows = fr;
		int cols = fc;

		while (cols != ec) {
			printf("%4d ", matrix[fr][cols]);
			cols++;
		}
		while (rows != er) {
			printf("%4d ", matrix[rows][ec]);
			rows++;
		}
		while (cols != fc) {
			printf("%4d ", matrix[er][cols]);
			cols--;
		}
		while (rows != fr) {
			printf("%4d ", matrix[rows][fc]);
			rows--;
		}
	}
	printf("\n");
}

/*
 * rows -> 行, 排
 * cols -> 列
 *
 * 左上角点 (fr, fc), 右下角点 (er, ec), 确定 4 条边
 *
 */
void spiral_order_print_matrix(int matrix[][COLS], int rows, int cols)
{
	int fr = 0, fc = 0;
	int er = rows - 1, ec = cols - 1;

	while (fr <= er && fc <= ec) {
		print_4_edges(matrix, fr++, fc++, er--, ec--);
	}
}

int main(int argc, char **argv)
{
	int matrix[][COLS] = { { 1, 2, 3, 4 }, { 5, 6, 7, 8 }, { 9, 10, 11, 12 }, { 13, 14, 15, 16 } };

	spiral_order_print_matrix(matrix, ARRAY_SIZE(matrix), COLS);

	return 0;
}
