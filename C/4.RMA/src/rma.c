/**
 * @file rma.c
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

	// Perform the all reduce.
	int value = my_rank;
	int reduced_value;
	MPI_Allreduce(&value, &reduced_value, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

	// Verify integrity
	if(my_rank == 0)
	{
		int reference_value = 0;
		for(int i = 0; i < comm_size; i++)
		{
			reference_value += i;
		}

		if(reference_value != reduced_value)
		{
			printf("The value reduced (%d) differs from the reference (%d).\n", reduced_value, reference_value);
			MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
		}
	}

	printf("[MPI process %d] Value reduced = %d.\n", my_rank, reduced_value);

	MPI_Finalize();

	return EXIT_SUCCESS;
}
