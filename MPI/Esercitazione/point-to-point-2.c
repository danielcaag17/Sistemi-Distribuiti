// Esercitazione point-to-point 2
/*
Process 0 reads a vector v[N] from a file and distributes chunks the same size to all the process
Each process sorts its vector an then return the maximum value to process 0
*/

#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

#define N 16

void printV(int v[], int size) {
	for (int i = 0; i < size; i++) printf("%d, ", v[i]);
	printf("\n");
}

int main(int argc, char *argv[]) {
	int nproc, myRank;
	int v[N];
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nproc);
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
	printf("Hello, I'm rank %d\n", myRank);

	if (myRank == 0) {
		FILE *fd = fopen("vector.txt", "r");
		if (fd == NULL) {
			printf("Error openning the file\n");
			return 0;
		}
		int i = 0;
		while(i < N && fscanf(fd, "%d", &v[i]) == 1) i++;
		fclose(fd);
		printV(v, N);
	}

	int sub_v[N/nproc];
	MPI_Scatter(&v, N/nproc, MPI_INT, &sub_v, N/nproc, MPI_INT, 0, MPI_COMM_WORLD);
	printV(sub_v, N/nproc);

	// Sort sub_v
	for (int i = 0; i < N/nproc; i++) {
		for (int j = i + 1; j < N/nproc; j++) {
			if (sub_v[i] < sub_v[j]) {
				int temp = sub_v[j];
				sub_v[j] = sub_v[i];
				sub_v[i] = temp;
			}
		}
	}
	printV(sub_v, N/nproc);
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

	// --------------- COMPARING RESULTS WITH REDUCE --------------------------
	int max_reduce[nproc];
	MPI_Reduce((void *) &sub_v, &max_reduce, nproc, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
	if (myRank == 0) printV(max_reduce, nproc);
	
	MPI_Finalize();
}
