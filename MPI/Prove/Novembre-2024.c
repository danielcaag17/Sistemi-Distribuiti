// A topology bidimensional n*n (nproc = n*n)
// Each process generate a number
// Process in the same row shift their value to the right -1 positions
// At this point the first process of each row gets the sum of the row

#include <stdio.h>
#include "mpi.h"

#define n 2

int rng(int rank) {
	return (rank*2 + 1) % 100;
}

int main(int argc, char *argv[]) {
	int myRank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
	printf("Hello, I'm rank %d\n", myRank);

	int dims[2], periods[2], reorder = 0;
	dims[0] = n; dims[1] = n;
	periods[0] = 0; periods[1] = 1;
	MPI_Comm comm_cart;
	MPI_Cart_create(MPI_COMM_WORLD, 2,dims, periods, reorder, &comm_cart);
	int myCoords[2], newRank;
	MPI_Comm_rank(comm_cart, &newRank);
	MPI_Cart_coords(comm_cart, newRank, 2, myCoords);
	printf("Hello, I'm rank %d with new rank %d and position %d,%d\n", myRank, newRank, myCoords[0], myCoords[1]);
	

	int random_number = rng(myRank);
	int destRank, sourceRank;
	MPI_Cart_shift(comm_cart, 1, -1, &newRank, &destRank);
	MPI_Cart_shift(comm_cart, 1, 1, &newRank, &sourceRank);
	printf("Hello, I'm rank %d, destRank is %d and sourceRank is %d\n", myRank, destRank, sourceRank);

	int recv_random = -1;
	MPI_Status status;
	// Deadlock
	// MPI_Sendrecv((void *) &random_number, 1, MPI_INT, destRank, MPI_ANY_TAG, (void *) &recv_random, 1, MPI_INT, sourceRank, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
	printf("Hello, I'm rank %d and I send %d and I recive %d\n", myRank, random_number, recv_random);

	for(int i = 0; i < n; i++) {
		if (myCoords[0] == i && myCoords[1] == 0) {
			int partial_sum;
			MPI_Reduce(&recv_random, &partial_sum, 1, MPI_INT, MPI_SUM, newRank, MPI_COMM_WORLD);
			printf("Hello, I'm rank %d the sum is %d\n", myRank, partial_sum);
		}
	}

	MPI_Finalize();
}
