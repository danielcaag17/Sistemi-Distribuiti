// Una matriz A(p*k) y una matriz B(k*m) de ints
// Proceso 0 lee las matrices y las distribuye a los otros procesos dividiendo las matriz A por filas y B por columnas
// Haz el producto de fila por columna de las matrices Ay B en la matriz C(p*m)
// Calcula dos vectores MINR[p] y MAXC[m] que contienen respectivamente los valores minimo de las filas de C y los valores maximos de las columnas de C
// Estos dos vectores se guardan en el proceso 0

#include <stdio.h>
#include "mpi.h"

#define p 4
#define m 4
#define k 4

int main (int argc, char* argv[]) {
	int size, myRank;
	int A[p][k], B[k][m];

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
	printf("Hello, I'm rank %d\n", myRank);

	int sub_a[p/size][k];
	int sub_b[m/size][k];
	if (myRank == 0){
		FILE *fda = fopen("matrixA.txt", "r");
		int i = 0;
		while (i < p*k && fscanf(fda, "%d", &A[i/k][i%p]) == 1) i++;
		fclose(fda);

		FILE *fdb = fopen("matrixB.txt", "r");
		i = 0;
		while (i < k*m && fscanf(fdb, "%d", &B[i/m][i%k]) == 1) i++;
		fclose(fdb);
		
		MPI_Scatter(&A, p*k/size, MPI_INT, &sub_a, p*k/size, MPI_INT, 0, MPI_COMM_WORLD);
	
		int BT[m][k];
		for (int i = 0; i < k; i++) {
			for (int j = 0; j < m; j++) BT[j][i] = B[i][j];
		}
		MPI_Scatter(&B, m*k/size, MPI_INT, &sub_b, m*k/size, MPI_INT, 0, MPI_COMM_WORLD);
	}

	int sub_bT[k][m/size];
	for (int i = 0; i < m/size; i++) {
		for (int j = 0; j < k; j++) sub_bT[j][i] = sub_b[i][j];
	}

	int Ci[p/size][m/size];
	for (int i = 0; i < p/size; i++) {
		for (int j = 0; j < m/size; j++) {
			Ci[i][j] = 0;
			for (int l = 0; l < k; l++) {
				Ci[i][j] += sub_a[i][l]*sub_bT[l][j];
			}
		}
	}

	int C[size/2][size/2];
	MPI_Gather(&Ci, (p*m)/(size*size), MPI_INT, &C, (p*m)/(size*size), MPI_INT, 0, MPI_COMM_WORLD);
	int MINR[size/2], MAXC[size/2];
	
	// rows and cols in diferents fors
	for (int i = 0; i < size/2; i++) {
		MINR[i] = 9999999;
		MAXC[i] = -999999;
		for (int j = 0; j < size/2; j++) {
			if (C[i][j] < MINR[i]) MINR[i] = C[i][j];
			if (C[i][j] > MAXC[i]) MAXC[i] = C[i][j];
		}
	}
	

	MPI_Finalize();
}
