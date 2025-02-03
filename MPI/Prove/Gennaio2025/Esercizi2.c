// Reorder a matrix
// Order the rows in function of the values of the first row (descending order)

#include <stdio.h>

#define p 4
#define k 2
#define DIM 32 // p = 4, nproc = 4, k = 2

int main(int argc, char *argv[]) {
	int matrix[DIM][DIM];

	FILE *fd = fopen("matrix.txt", "r");
	for (int i = 0; i < DIM; i++) {
		for (int j = 0; j < DIM; j++) {	
			fscanf(fd, "%d", &matrix[i][j]);

			// Verify it has been read correctly 
			// printf("%d ", matrix[i][j]);
		}
		// printf("\n");
	}
	
	for (int i = 0; i < DIM; i++) {		// DIM here is refered to column length
		for (int j = 0; j < DIM - i - 1; j++) {	// DIM refered to column length
			if (matrix[j][0] < matrix[j + 1][0]) {	// DIM referen to row length
				for (int l = 0; l < DIM; l++) {
					int tmp = matrix[j][l];
					matrix[j][l] = matrix[i][l];
					matrix[i][l] = tmp;
				}
			}
		}
	}
	
	// Show the result
	for (int i = 0; i < DIM; i++) {
		for (int j = 0; j < DIM; j++) {
			printf("%d ", matrix[i][j]);
		}
		printf("\n");
	}
}
