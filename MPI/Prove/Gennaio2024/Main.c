// Gennaio 2024

#include "mpi.h"
#include <stdio.h>

#define nproc 4
#define DIM 16

int
main (int argc, char *argv[])
{
	int rank, size;
	
	MPI_Init (&argc, &argv);
	MPI_Comm_size (MPI_COMM_WORLD, &size); // instead of size we always use nproc (4)
	MPI_Comm_rank (MPI_COMM_WORLD, &rank);
	
	// In this example, DIM = 16
	// Each process receives 4 elements
	// As we want to receive the elements of k blocks (DIM/nproc*2)
	// Each process receives 2 elements and then, after moving the vector 6 elements, the process recives the 2 last elements
  	int vector[DIM];
	if (rank == 0) {
		for (int i = 0; i < DIM; i++) vector[i] = i;	
	}
	
	int sub_vector1[DIM/(nproc*2)], sub_vector2[DIM/(nproc*2)];
	MPI_Scatter (&vector[0], DIM / (nproc * 2), MPI_INT, &sub_vector1, DIM / (nproc * 2), MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Scatter (&vector[DIM / 2], DIM / (nproc * 2), MPI_INT, &sub_vector2, DIM / (nproc * 2), MPI_INT, 0, MPI_COMM_WORLD);
	
	printf ("I'm process %d \n", rank);
	for (int i = 0; i < DIM / (nproc*2); i++) printf ("%d ", sub_vector1[i]);
	for (int i = 0; i < DIM / (nproc*2); i++) printf ("%d ", sub_vector2[i]);
	printf ("\n");   
	
	// -------------------------------------------------------------
	
	int A[2][DIM/2];
	for (int i = 0; i < DIM/2; i++) {
		A[0][i] = sub_vector1[i];
		A[1][i] = sub_vector2[i];
	}
	
	for (int i = 0; i < DIM/(nproc*2); i++) {
		for (int j = 0; j < DIM/(nproc*2) - i; j++) {
			if (A[0][j] > A[0][i]) {
				int aux = A[0][j];
				A[0][j] = A[0][i];
				A[0][i] = aux;
			}
			if (A[1][j] > A[1][i]) {
				int aux = A[1][j];
				A[1][j] = A[1][i];
				A[1][i] = aux;
			}
		}
	} 
	  
	printf ("I'm process %d \n", rank);            
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < DIM/(nproc*2); j++) printf("%d ", A[i][j]);
		printf("\n");
	}
	
	// ---------------------------------------------------------------

	int res = 0;
	if (A[0][0] < A[1][0]) {
		for (int i = 0; i < DIM/(nproc*2); i++) res += A[0][i];
	} else {
		res = 1;
		for (int i = 0; i < DIM/(nproc*2); i++) res *= A[1][i];
	}
	printf("I'm process %d and res=%d \n", rank, res);
	
	        
	MPI_Finalize ();
}

