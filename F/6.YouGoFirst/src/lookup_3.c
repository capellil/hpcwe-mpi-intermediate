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

	// Find other MPI processes on the same node
	MPI_Comm intranodecomm;
	MPI_Comm_split_type(MPI_COMM_WORLD, MPI_COMM_TYPE_SHARED, my_rank, MPI_INFO_NULL, &intranodecomm);

	// Get my rank in the intranode communicator
	int intranoderank;
	MPI_Comm_rank(intranodecomm, &intranoderank);

	// Each MPI process provides a window with its elements
	MPI_Win win;
	MPI_Win_allocate_shared(values, sizeof(int) * count, sizeof(int), MPI_INFO_NULL, intranodecomm, &win);
	int size;
	int disp_unit;
	int* values = NULL;
	MPI_Win_shared_query(win, 0, &size, &disp_unit &values);
	// To access local elements: values[...]
	// To access remote elements: values[...]
	// Problems: none left
	MPI_Win_free(win);

	MPI_Finalize();

	return EXIT_SUCCESS;
}
