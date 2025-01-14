// Esercitazione collective 2
/*
Process 0 reads a vector v[N] from a file and distributes chunks the same size to all the process
Each process uses all gather to the other processes so each process now have a matrix m[nproc][N/nproc]
Now each process does the transposed matrix and multiplies the Ith row of m with the Ith column of mT, the result is stored in xI[nproc]
Each process calculates the max value of xI and then passes the result to process 0
Process 0 reconstructs vector X in the sense that the order is according to the max values
*/

#include "mpi.h"
#include <stdlib.h>
#include <stdio.h>

#define N 16

void printV(int v[], int size) {
	for (int i = 0; i < size; i++) printf("%d ,,", v[i]);
	printf("\n");
}

int main(int argc, char* argv[]) {
	int myRank, nproc;
	int v[N];

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nproc);
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
	printf("Hello, I'm rank %d\n", myRank);

	if (myRank == 0) {
		FILE *fd = fopen("vector.txt", "r");
		int i = 0;
		while(i<N && fscanf(fd, "%d", &v[i]) == 1) i++;
		printV(v, N);
		
	}
	int sub_v[N/nproc];
	MPI_Scatter(&v, N/nproc, MPI_INT, &sub_v, N/nproc, MPI_INT, 0, MPI_COMM_WORLD);
	printV(sub_v, N/nproc);

	int m[nproc][N/nproc];
	MPI_Allgather(sub_v, N, MPI_INT, m, N, MPI_INT, MPI_COMM_WORLD);
	
	for (int i = 0; i < nproc; i++) {
		for (int j = 0; j < N/nproc; j++) printf("%d ", m[i][j]);
		printf("\n");
	}
	
	int mT[N/nproc][nproc];
	for (int i = 0; i < nproc; i++) {
		for (int j = 0; j < N/nproc; j++) mT[j][i] = m[i][j];
	}

	int xI[nproc];
	for (int i = 0; i < nproc; i++) xI[i] = 0;	
	for (int i = 0; i < nproc; i++) xI[i] += m[myRank][i]*mT[i][myRank];

	int max_value = -999999;
	for (int i = 0; i < nproc; i++) {
		if (xI[i] > max_value) max_value = xI[i];
	}
	printf("I'm rank %d and the max value is %d\n", myRank, max_value);

	MPI_Barrier(MPI_COMM_WORLD);
	if (myRank == 0) {
		int max_values[nproc];
		int tmp;
		MPI_Status status;
		for (int i = 1; i < nproc; i++){
			MPI_Recv((void *) &tmp, 1, MPI_INT, i, 1, MPI_COMM_WORLD, &status);
			max_values[i] = tmp;
			printf("Message recieved from rank %d and max=%d\n", i, max_values[i]);
		}
		max_values[0] = sub_v[0];
		printV(max_values, nproc);
	}
	else{
		MPI_Send((void *) &sub_v, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
	}
	
	


	MPI_Finalize();
}
