// Maggio 2022

#include "mpi.h"
#include <stdio.h>
#define DIM 24

int main (int argc, char *argv[]) {
  int rank, size;

  MPI_Init (&argc, &argv);
  MPI_Comm_rank (MPI_COMM_WORLD, &rank);
  MPI_Comm_size (MPI_COMM_WORLD, &size);

  int v[DIM];
  if (rank == 0) {
      for (int i = 0; i < DIM; i++) v[i] = i;
  }

  int sub_vector1[DIM / (size * 2)], sub_vector2[DIM / (size * 2)];
  MPI_Scatter (&v[0], DIM / (size * 2), MPI_INT, sub_vector1, DIM / (size * 2), MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Scatter (&v[DIM / 2], DIM / (size * 2), MPI_INT, sub_vector2, DIM / (size * 2), MPI_INT, 0, MPI_COMM_WORLD);

  int A[DIM / (size * 2)][2];
  for (int i = 0; i < DIM / (size * 2); i++) {
      A[i][0] = sub_vector1[i];
      A[i][1] = sub_vector2[i];
  }

  MPI_Barrier (MPI_COMM_WORLD);
  printf ("I'm process %d, showing A\n", rank);
  for (int i = 0; i < DIM / (size * 2); i++) {
      for (int j = 0; j < 2; j++) printf ("%d ", A[i][j]);
      printf ("\n");
  }

// ------- TOPOLOGIA ----------------
  struct {
    int value;
    int rank;
  } local_min, global_min_loc;
  global_min_loc.rank = -1;
  if (size % 2 != 0) {
      local_min.value = A[0][0];
      local_min.rank = rank;
      MPI_Allreduce (&local_min, &global_min_loc, 1, MPI_2INT, MPI_MINLOC, MPI_COMM_WORLD);
      printf ("The process eliminated is %d\n", global_min_loc.rank);
  }

  if (rank != global_min_loc.rank) {
      int dims[2] = { 2, size / 2 };
      int periods[2] = { 0, 0 }; // Non periodic
      int reorder = 0;
      MPI_Comm comm_cart;
      MPI_Cart_create (MPI_COMM_WORLD, 2, dims, periods, reorder, &comm_cart);

// ------------------------------
      int myCoords[2];
      MPI_Cart_coords (comm_cart, rank, 2, myCoords);
      MPI_Barrier (comm_cart);
      printf ("I'm process %d and my coords are %d,%d\n", rank, myCoords[0], myCoords[1]);

      int down, up;
      MPI_Cart_shift (comm_cart, 0, 1, &up, &down);

      if (myCoords[0] == 0) { // First row
		 int value = A[0][0];
		 printf ("I'm process %d and I send %d to %d\n", rank, value, down);
		 MPI_Send (&value, 1, MPI_INT, down, 0, MPI_COMM_WORLD);
	 }
	  else { // Second row
		 int value;
		 MPI_Status status;
		 MPI_Recv (&value, 1, MPI_INT, up, 0, MPI_COMM_WORLD, &status);
		 printf ("I'm process %d and I receive %d from %d\n", rank, value, up);
		 A[0][0] = value;
	  }
  }

  MPI_Finalize ();
}
