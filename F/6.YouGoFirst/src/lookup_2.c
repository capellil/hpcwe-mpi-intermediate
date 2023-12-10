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

    // Distribute it across MPI processes
	int modulo = 1000000 % comm_size;
	int count = (1000000 - modulo) / comm_size;
	int my_values[count];
	if(my_rank == 0)
	{
		int values[100000];
		MPI_Scatter(&values[modulo], my_values, count, MPI_INT, 0, MPI_COMM_WORLD);
		count += modulo;
	}
	else
	{
		MPI_Scatter(NULL, my_values, count, MPI_INT, 0, MPI_COMM_WORLD);
	}

	// Each MPI process provides a window with its elements
	MPI_Win win;
	MPI_Win_create(values, sizeof(int) * count, sizeof(int), MPI_INFO_NULL, MPI_COMM_WORLD, &win);
	MPI_Win_fence(0, win);
	// To access local elements, my_values[...]
	// To access remote elements, MPI_Get(...)
	// Problem: must use temporary buffers for remote values + fetching time.
	MPI_Win_fence(0, win);
	MPI_Win_free(win);

	MPI_Finalize();

	return EXIT_SUCCESS;
}
