// Create a topology bidimensional (2x2, 4 processes)
// Show your rank before and after the topology creation
// Show your coords and also show your neighbours coords and ranks
// Do the same as before but now the topology is not periodic

#include "mpi.h"
#include <stdio.h>

int main(int argc, char* argv[]) {
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
	printf("Hello, there are %d processes and my rank id %d\n", size, rank);
	
	// TOPOLGOY CREATION - PERIODIC
	int ndims = 2;
	// int dims[2] = {size/2, size/2};
	int dims[2] = {4, 1};
	int periods[2] = {0, 0};
	int reorder = 0;
	MPI_Comm comm_cart;
	MPI_Cart_create(MPI_COMM_WORLD, ndims, dims, periods, reorder, &comm_cart);
	
	int myCoords[2];
	MPI_Cart_coords(comm_cart, rank, 2, myCoords); // To know your coords
	MPI_Barrier(comm_cart); // Syncrhonize all processes
	printf("I'm rank %d and my coords are [%d][%d]\n", rank, myCoords[0], myCoords[1]);
	
	// Discover the neighbours
	int left, right, up, down;
	MPI_Cart_shift(comm_cart, 1, 1, &left, &right);
	MPI_Cart_shift(comm_cart, 0, 1, &up, &down);
	MPI_Barrier(comm_cart);
	printf("I'm rank %d and my neighbours are: left(%d), right(%d), up(%d), down(%d)\n", rank, left, right, up, down);
	
	int cart_rank;
	MPI_Cart_rank(comm_cart, myCoords, &cart_rank); // To know the rank given your coords
	
	MPI_Barrier(comm_cart);
	if (rank == cart_rank) printf("The ranks are the same (%d)\n", rank);
	else printf("The carts are different, old_rank is %d and new rank is %d\n", rank, cart_rank);
	
	MPI_Finalize();

}
