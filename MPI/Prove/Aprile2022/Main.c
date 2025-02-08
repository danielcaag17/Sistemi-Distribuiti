// Aprile 2022

#include "mpi.h"
#include <stdio.h>

#define DIM 8

int main (int argc, char *argv[]) {
	int rank, size;
	
	MPI_Init (&argc, &argv);
	MPI_Comm_rank (MPI_COMM_WORLD, &rank);
	MPI_Comm_size (MPI_COMM_WORLD, &size);
	
	int matrix[DIM][DIM];
	if (rank == 0) {
		for (int i = 0; i < DIM; i++) {
 			for (int j = 0; j < DIM; j++) matrix[i][j] = i * DIM + j;
		}
    }
	int sub_matrix[DIM][DIM / size];
	MPI_Scatter (&matrix, DIM * DIM / size, MPI_INT, &sub_matrix, DIM * DIM / size, MPI_INT, 0, MPI_COMM_WORLD);

	MPI_Barrier (MPI_COMM_WORLD);
	printf ("I'm process %d and this is my matrix\n", rank);
	for (int i = 0; i < DIM; i++) {
		for (int j = 0; j < DIM / size; j++) {
			printf ("%d ", sub_matrix[i][j]);
		}
	 	 printf ("\n");
	}

	// ---------------------------------------------------------------------

	int v_max[DIM];
	for (int i = 0; i < DIM; i++) {
	  	int aux = -1;
	  	for (int j = 0; j < DIM / size; j++) {
			if (sub_matrix[i][j] > aux) aux = sub_matrix[i][j];
		}
	  	v_max[i] = aux;
	}

	MPI_Barrier (MPI_COMM_WORLD);
	printf ("I'm process %d and this is my max vector\n", rank);
	for (int i = 0; i < DIM; i++) {
		printf ("%d ", v_max[i]);
	}
	printf ("\n");

// ---------------------------------------------------------------------

	int v_max_global[size];
	int value = v_max[0];
	MPI_Gather (&value, 1, MPI_INT, &v_max_global, 1, MPI_INT, 0, MPI_COMM_WORLD);
	
	int chosen = 0;
	MPI_Barrier (MPI_COMM_WORLD);
	if (rank == 0) {
		printf ("I'm rank 0 and this is the v_max_global\n");
		for (int i = 0; i < size; i++) printf ("%d ", v_max_global[i]);
		printf ("\n");
	
		int chosens[6];
		for (int i = 0; i < 6; i++)	{
			int value = -1;
			for (int j = 0; j < size; j++) {
				if (v_max_global[j] > value) {
					value = v_max_global[j];
					chosens[i] = j;
				}
			}
			v_max_global[chosens[i]] = -1;
			if (chosens[i] == 0) chosen = 1;
			else {
				int aux = 1;
				printf ("I send chosen to rank %d\n", chosens[i]);
				MPI_Send (&aux, 1, MPI_INT, chosens[i], 0, MPI_COMM_WORLD);
			}
		}
	
		// Send a message to the other waiting processes
		for (int i = 0; i < size; i++) {
			int sended = 0;
			for (int j = 0; j < 6; j++) {
				if (chosens[j] == i) sended = 1;
			}
			if (sended == 0) {
				printf ("I send not chosen to rank %d\n", i);
				if (i != 0) MPI_Send (&sended, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
				else chosen = 0;
			}
		}
	}
	else {
		MPI_Status status;
		MPI_Recv (&chosen, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
	}

	if (chosen == 1) {
		printf ("I'm rank %d and I'm chosen\n", rank);
		int dims[2] = { 2, 3 };
		int periods[2] = { 0, 0 }; // Non periodic
		int reorder = 0;
		MPI_Comm comm_cart;
		MPI_Cart_create (MPI_COMM_WORLD, 2, dims, periods, reorder, &comm_cart);
		
		int myCoords[2];
		MPI_Cart_coords (comm_cart, rank, 2, myCoords);
		
		MPI_Barrier (comm_cart);
		printf ("I'm rank %d and my coords are %d,%d\n", rank, myCoords[0],
		myCoords[1]);	
	}
	else {
		printf ("I'm rank %d and I'm not chosen\n", rank);
    }

  MPI_Finalize ();
}
