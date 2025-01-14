// round robin with MPI
// There are 4 process and each one passes a random number to the next one (process_id + 1)

#include "mpi.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

void round_robin(int rank, int num_procs) {
	int next_rank, my_number, prev_rank, prev_number;
	MPI_Status status;

	next_rank = (rank + 1) % num_procs;
	if (rank == 0) prev_rank = num_procs - 1;
	else prev_rank = rank - 1;

	srand(time(NULL) + rank);
	my_number = rand() / (RAND_MAX / 100);

	if (rank % 2 == 0) {
		printf("%d: I send %d to rank %d\n", rank, my_number, next_rank);
		MPI_Send((void *) &my_number,  1, MPI_INT, next_rank, 1, MPI_COMM_WORLD);

		MPI_Recv((void *) &prev_number, 1, MPI_INT, prev_rank, 1, MPI_COMM_WORLD, &status);
		printf("%d: I recive %d from rank %d\n", rank, prev_number, prev_rank);
	}
	else {
		MPI_Recv((void *) &prev_number, 1, MPI_INT, prev_rank, 1, MPI_COMM_WORLD, &status);
		printf("%d: I recive %d from rank %d\n", rank, prev_number, prev_rank);

		printf("%d: I send %d to rank %d\n", rank, my_number, next_rank);
		MPI_Send((void *) &my_number,  1, MPI_INT, next_rank, 1, MPI_COMM_WORLD);

	}
}

int main (int argc, char* argv[]) {
	int rank;
	int num_procs;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	printf("%d: this is my rank of %d\n", rank, num_procs);
	round_robin(rank, num_procs);
	printf("%d: round robin finalized\n", rank);


	MPI_Finalize();      
}
