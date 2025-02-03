// read a matrix from matrix.txt, then process 0 sends it to the other processes
// each process receives k*nproc rows p times, but no consecutive
// k*nproc rows are distributed among all process then it distributed againg k*nproc rows p times.

// Reorder a matrix
// Order the rows in function of the values of the first row (descending order)

// Calculate the max in matrix[0][0] among all the process
// The process with max value send its row to all the other processes

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

	// --------------- ESERCIZI 2 -------------------------------------------
	for (int i = 0; i < k*p; i++) {				// Length of the column
		for (int j = 0; j < (k*p) - i - 1; j++) {	// Length of the column
			if (sub_matrix[j][0] < sub_matrix[j + 1][0]) {	// DIM referen to row length
				for (int l = 0; l < DIM; l++) {
					int tmp = sub_matrix[j][l];
					sub_matrix[j][l] = sub_matrix[i][l];
					sub_matrix[i][l] = tmp;
				}
			}
		}
	}
	
		// Verify each process has the correct part of the matrix
	for (int l = 0; l < size; l++) {
		if (l == rank) {
			printf("\nESERCIZI 2\n");
			for (int i = 0; i < k*p; i++) {
				for (int j = 0; j < DIM; j++) {
					printf("%d ", sub_matrix[i][j]);
				}
				printf("\n");
			}
			printf("I'm process %d and I showed my sub_matrix ordered\n", rank);
		}
		// Print one process after the other
		MPI_Barrier(MPI_COMM_WORLD);
	}

	// --------------- ESERCIZI 3 -------------------------------------------
	struct {
		int value;
		int rank;
	} local_max, global_max_loc;
	local_max.value = sub_matrix[0][0];
	local_max.rank = rank;
	
	MPI_Allreduce(&local_max, &global_max_loc, 1, MPI_2INT, MPI_MAXLOC, MPI_COMM_WORLD);
	printf("Global max is %d and the rank is %d\n", global_max_loc.value, global_max_loc.rank);

	int first_row[DIM];
	if (local_max.value == global_max_loc.value) {
		for (int i = 0; i < DIM; i++) first_row[i] = sub_matrix[0][i];
	}	

	MPI_Bcast(&first_row, DIM, MPI_INT, global_max_loc.rank, MPI_COMM_WORLD);
	
	for (int i = 0; i < DIM; i++) {
		printf("%d ", first_row[i]);
	}
	printf("\n");

	

	MPI_Finalize();

}
