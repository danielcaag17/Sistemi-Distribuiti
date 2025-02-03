#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DIM 32


int main() {
	FILE *fd = fopen("matrix.txt", "w");
	srand(time(NULL));
	for (int i = 0; i < DIM; i++) {
		for (int j = 0; j < DIM; j++) {	
			int random_number = rand() % 100;
			fprintf(fd, "%d", random_number);
			fprintf(fd, " ");
		}
		fprintf(fd, "\n");
	}
	fclose(fd);
}
