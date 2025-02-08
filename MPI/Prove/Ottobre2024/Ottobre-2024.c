// The process 0 reads a matrix M[DIM][DIM] and distributes blocks of size k to the rest of processes (DIM multiple of k*nproc)
// Each process sorts the rows
// The maximum value v[0][0] of all process are passed to all of them

#include <stdio.h>
#include "mpi.h"

#define k 4

void read_matrix(int *m, int size) {
	for (int i = 0; i < k*size; i++) {
		for (int j = 0; j < k*size; j++) {
			m[i*k*size+j] = i*k*size + j;
		}
	}
}

void print_matrix(int *m, int rows, int cols) {
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			printf("%d ", m[i*cols+j]);
		}
		printf("\n");
	}
}

void sort(int *v, int size) {
	for (int i = 0; i < size; i++) {
		for (int j = i + 1; j < size - i -1; j++) {
			if (v[i] < v[j]) {
				int temp = v[j];
				v[j] = v[i];
				v[i] = temp;
			}
		}
	}
	//printf("\n\n\n");
	//for (int i = 0; i < size; i++) printf("%d ", v[i]);
}

int main(int argc, char *argv[]) {
	int size, myRank;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
	printf("Hello, I'm rank %d\n", myRank);

	int m[k*size][k*size];
	if (myRank == 0) {
		read_matrix(m, size);
		// print_matrix(m, k*size, k*size);
	}

	int sub_mat[k][k*size];
	MPI_Scatter(&m, k*size*k*size/size, MPI_INT, &sub_mat, k*size*k*size/size, MPI_INT, 0, MPI_COMM_WORLD);

	if (myRank == 0) print_matrix(sub_mat, k, k*size);
	MPI_Barrier(MPI_COMM_WORLD);
	if (myRank == 1) print_matrix(sub_mat, k, k*size);

	
	for (int i = 0; i < k; i++) {
		int v[k*size];
		for (int j = 0; j < k*size; j++) v[j] = sub_mat[i][j];
		sort(v, k*size);
		printf("%d\n\n\n", v[0]);
		for (int j = 0; j < k*size; j++) sub_mat[i][j] = v;
	}

	if (myRank == 0) print_matrix(sub_mat, k, k*size);
	MPI_Barrier(MPI_COMM_WORLD);		// Para que no se solapen en los printf, serializar el codigo
	if (myRank == 1) print_matrix(sub_mat, k, k*size);

	int max_value;
	printf("%d\n\n", sub_mat[0][0]);
	MPI_Allreduce(&sub_mat[0][0], max_value, 1, MPI_INT, MPI_MAX, MPI_COMM_WORLD);
	printf("Hello, I'm rank %d and max value is %d\n", myRank, max_value);


	MPI_Finalize();
}
