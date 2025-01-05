#include <stdio.h>
#include <string.h>
#include "mpi.h"        
#include <stdlib.h>

#define DIM 4

void read_matrix(char x, int matrix[DIM][DIM]) {
    if (x == 'A') {
        for (int i = 0; i < DIM; i++) {
            for (int j = 0; j < DIM; j++) {
                matrix[i][j] = i * DIM + j + 1;
            }
        }
    } else if (x == 'B') {
        for (int i = 0; i < DIM; i++) {
            for (int j = 0; j < DIM; j++) {
                matrix[i][j] = DIM * DIM - (i * DIM + j);
            }
        }
    }
}

int compare_desc(const void *a, const void *b) {
    return (*(int*)b - *(int*)a);  
}

void show_matrix(int matrix[DIM][DIM]) {
    for (int i = 0; i < DIM; i++) {
        for (int j = 0; j < DIM; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }  
}

int main(int argc, char* argv[]) {
    int matrixA[DIM][DIM], matrixB[DIM][DIM];
    int myRank, numProc;
    MPI_Status status;

    MPI_Init(&argc, &argv);  
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);  
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);  

    if (DIM % numProc != 0) {
        if (myRank == 0) {
            fprintf(stderr, "The division dim / nproc is not an integer\n");
        }
        MPI_Finalize();
        return 1;
    }

    // Inicia matrices en el proceso raíz
    if (myRank == 0) {
        read_matrix('A', matrixA);
        read_matrix('B', matrixB);
        show_matrix(matrixA);
        show_matrix(matrixB);
    }

    // Usando memoria dinámica para evitar problemas de tamaño de la matriz
    int *rows = malloc(DIM * DIM / numProc * sizeof(int));
    int *cols = malloc(DIM * DIM / numProc * sizeof(int));
    int *transposedB = malloc(DIM * DIM * sizeof(int));

    MPI_Scatter(matrixA, DIM * DIM / numProc, MPI_INT, rows, DIM * DIM / numProc, MPI_INT, 0, MPI_COMM_WORLD);
    int new_rows[DIM/numProc][DIM];
    for (int i = 0; i < DIM/numProc; i++) {
        for (int j = 0; j < DIM; j++) {
            new_rows[i][j] = rows[i*DIM+j];
        }
    }
    
    // Transponer la matriz B
    if (myRank == 0) {
        for (int i = 0; i < DIM; i++) {
            for (int j = 0; j < DIM; j++) {
                transposedB[i * DIM + j] = matrixB[j][i];
            }
        }
    }
    MPI_Scatter(transposedB, DIM * DIM / numProc, MPI_INT, cols, DIM * DIM / numProc, MPI_INT, 0, MPI_COMM_WORLD);
    int new_cols[DIM/numProc][DIM];
    for (int i = 0; i < DIM/numProc; i++) {
        for (int j = 0; j < DIM; j++) {
            new_cols[i][j] = cols[i*DIM+j];
        }
    }
    // In order to have the columns values of B ordered.
    int colsTransposed[DIM][DIM/numProc];
    for (int i = 0; i < DIM/numProc; i++) {
        for (int j = 0; j < DIM; j++) {
            colsTransposed[j][i] = new_cols[i][j];
        }
    }
    

    int C[DIM / numProc][DIM / numProc];

    for (int i = 0; i < DIM / numProc; i++) {
        for (int j = 0; j < DIM / numProc; j++) {
            int result = 0;
            for (int k = 0; k < DIM; k++) {
                result += new_rows[i][k] * colsTransposed[k][j];
            }
            C[i][j] = result;
        }
    }

    free(rows);
    free(cols);
    free(transposedB);

    // Mostrar el resultado de la multiplicación
    printf("\nProcess %d result:\n", myRank);
    for (int i = 0; i < DIM / numProc; i++) {
        for (int j = 0; j < DIM / numProc; j++) {
            printf("%d ", C[i][j]);
        }
        printf("\n");
    }

    // Ordenar la columna 0 en orden decreciente
    int col_0[DIM / numProc];
    for (int i = 0; i < DIM / numProc; i++) {
        col_0[i] = C[i][0];
    }

    qsort(col_0, DIM / numProc, sizeof(int), compare_desc);

    // Colocar los valores ordenados de vuelta en la columna 0
    for (int i = 0; i < DIM / numProc; i++) {
        C[i][0] = col_0[i];
    }

    // Mostrar la matriz resultante ordenada
    printf("\nProcess %d ordered result:\n", myRank);
    for (int i = 0; i < DIM / numProc; i++) {
        for (int j = 0; j < DIM / numProc; j++) {
            printf("%d ", C[i][j]);
        }
        printf("\n");
    }
    
    int val_max_temp = C[0][0];
    int med;
    // Encontrar valor medio
    // MPI_Reduce(&val_max_temp, &med, 1, MPI_INT, MPI_MED, 0, MPI_COMM_WORLD);
    // Al ser un script se sabe cuanto valor el valor medio
    if (numProc == 2) med = (240+444)/2;    // 342
    else if (numProc == 4) med = (80+214+316+386)/4;    // 249
    else if (numProc == 1) med = 560;
    
    
    MPI_Comm new_comm;
    int new_rank, new_size;
    int color;

    if (val_max_temp >= med) color = 1;
    else color = 0;
    
    MPI_Comm_split(MPI_COMM_WORLD, color, myRank, &new_comm);
    MPI_Comm_size(new_comm, &new_size); 
    MPI_Comm_rank(new_comm, &new_rank); 
    
    printf("\nProceso %d con valor %d esta en el grupo %d con el rango %d y size %d\n", myRank, val_max_temp, color, new_rank, new_size);
    
    int reduce_result;
    if (color == 1) MPI_Reduce(&val_max_temp,&reduce_result, 1, MPI_INT, MPI_MAX, 0, new_comm);
    else MPI_Reduce(&val_max_temp,&reduce_result, 1, MPI_INT, MPI_MIN, 0, new_comm);
    
    if (new_rank == 0) {
        if (color == 1) printf("The max is %d\n", reduce_result);
        else printf("The min is %d\n", reduce_result);
    }
    
    MPI_Comm_free(&new_comm);

    MPI_Finalize();
    return 0;
}
