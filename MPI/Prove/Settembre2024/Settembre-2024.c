// Process 0 reads a vector A[DIM] and distribuites k elements to the other processes (DIM is multiple of k*nproc)
// Each process orders its vector
// The process 0 gets in a new vector de 5 different biggest values

#include <stdio.h>
#include "mpi.h"

#define k 4

void sort(int *v) {

}

int main(int argc, char *argv[]) {
	int nproc, myRank;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nproc);
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
	printf("Hello, I'm rank %d\n", myRank);

	int A[k*nproc];
	if (myRank == 0) {
		for (int i = 0; i < nproc*k; i++) {
			A[i] = i;
		}
	}

	int v[k];
	MPI_Scatter(&A, k, MPI_INT, &v, k, MPI_INT, 0, MPI_COMM_WORLD);
	
	sort(v);

	int maximum[5];
	MPI_Reduce(&v, &maximum, 5, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
	if (myRank == 0) {
		for (int i = 0; i < 5; i++) {
			printf("%d ", maximum[i]);
		}
		printf("\n");
	}


	MPI_Finalize();
}
