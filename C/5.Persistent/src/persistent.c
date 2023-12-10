/**
 * @file preamble.c
 * @brief This example is to practice with one-sided communications.
 * @details You will find two folders in this example, and will in all examples
 * provided in this session:
 * - src: contains the source code.
 * - bin: contains the binary produced.
 *
 * The makefile provided already sets everything up for you:
 * - To compile: `make`.
 * - To execute: `sbatch submission.slurm`.
 *
 * If you have any questions, do not hesitate.
 * @author Ludovic Capelli (l.capelli@epcc.ed.ac.uk)
 **/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

/**
 * @brief This implements an MPI all reduction using MPI_Allreduce. The 
 * objective is to rewrite it using one-sided communications instead.
 **/
int main(int argc, char* argv[])
{
	MPI_Init(&argc, &argv);

	// Get basic information
	int my_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	int comm_size;
	MPI_Comm_size(MPI_COMM_WORLD, &comm_size);

	int count = 0;
	int total = 0;
	for(int i = 0; i < 1000000; i++)
	{
		MPI_Reduce(&my_rank, &total, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
		count++;
	}
	printf("[MPI process %d] %d rounds done.\n", my_rank, count);

	MPI_Finalize();

	return EXIT_SUCCESS;
}
