// read a matrix from matrix.txt, then process 0 sends it to the other processes
// each process receives k*nproc rows p times, but no consecutive
// k*nproc rows are distributed among all process then it distributed againg k*nproc rows p times.

#include <stdio.h>
#include "mpi.h"

#define p 4
#define k 2
#define DIM 32 // p = 4, nproc = 4, k = 2

int main(int argc, char *argv[]) {
	int rank, size;
	int matrix[DIM][DIM];
	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	int sub_matrix[k*p][DIM];
	
	// Process 0 reads the matrix
	if (rank == 0) {
		FILE *fd = fopen("matrix.txt", "r");
		for (int i = 0; i < DIM; i++) {
			for (int j = 0; j < DIM; j++) {	
				fscanf(fd, "%d", &matrix[i][j]);

				// Verify it has been read correctly 
				// printf("%d ", matrix[i][j]);
			}
			// printf("\n");
		}		
	}

	// Send the matrix to the other processes
	for (int i = 0; i < p; i++) {
			// Matrix divided in p parts
			int aux[DIM/p][DIM];

			// Copy from matrix to aux the p part is going to be sent
			for (int j = 0; j < size*k; j++) {
				for (int l = 0; l < DIM; l++) {
					aux[j][l] = matrix[j + size*k*i][l];
					// printf("%d ", aux[j][l]);
				}
				// printf("\n");
			}
			// Send k rows, and each time sub_matrix starts k * i rows later
			MPI_Scatter(&aux, k*DIM, MPI_INT, &sub_matrix[k*i], k*DIM, MPI_INT, 0, MPI_COMM_WORLD);
		}	


	printf("I'm process %d\n", rank);
	MPI_Barrier(MPI_COMM_WORLD);		// This barrier is not necessary. Just wait process 0 before moving on

	
	// Verify each process has the correct part of the matrix
	for (int l = 0; l < size; l++) {
		if (l == rank) {
			for (int i = 0; i < k*p; i ++) {
				for (int j = 0; j < DIM; j++) {
					printf("%d ", sub_matrix[i][j]);
				}
				printf("\n");
			}
			printf("I'm process %d and I showed my sub_matrix\n", rank);
		}
		// Print one process after the other
		MPI_Barrier(MPI_COMM_WORLD);
	}
	

	MPI_Finalize();

}
