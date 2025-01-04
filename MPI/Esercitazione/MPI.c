/*
To compile and execute with 4 process in parallel:

# mpicc MPI.c -o MPI
# mpirun -np 4 ./MPI

*/


#include <stdio.h>
#include <string.h>
#include "mpi.h"        // includes MPI library code specs

#define MAXSIZE 100

int main(int argc, char* argv[]) {
    int myRank;         // rank (identity) of process
    int numProc;        // number of processors
    int source;         // rank of sender
    int dest;           // rank of destination
    int tag = 0;        // tag to distinguish messages
    char mess[MAXSIZE]; // message (other types possible)
    int count;          // number of items in message
    MPI_Status status;  // status of message received

    MPI_Init(&argc, &argv);     // start MPI

    // get number of precesses
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);

    // get rank of this process
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

    // HERE starts the part of the code to send, receive and process messages

    if (myRank != 0) {  // processes that are not root
        // create message
        sprintf(mess, "Hello from %d", myRank);
        dest = 0;          // destination is root
        count = strlen(mess) + 1;   // +1 to include '\0' in message

        // send mess to dest
        MPI_Send(mess, count, MPI_CHAR, dest, tag, MPI_COMM_WORLD);

        /*
        int x;
        printf("process %d: give me a number:", myRank);
        scanf("%d", x);

        */
    }
    else {              // root process (0)
        // receive mess from each processor in rank order
        for(source = 1; source < numProc; source++) {
        MPI_Recv(mess, MAXSIZE, MPI_CHAR, source, tag, MPI_COMM_WORLD, &status);
        printf("%s\n", mess);
    }

    /*
    do {
        MPI_Recv(&x, 1, MPI_INT, MPI_ANY_SOURCE, type, MPI_COMM_WORLD, &status);
        printf("process &d: firo numero: %d\n", rank, x);
        if (x == 0 )
    }
    */
    }

    // HERE ends the part of the code to send, receive and process messages

    MPI_Finalize();      // shut down MPI
}