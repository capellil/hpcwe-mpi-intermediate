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

    const int INDEX_MY_VALUE = 0;
    const int INDEX_TOTAL_VALUE = 1;
    int values[2];
    values[INDEX_MY_VALUE] = my_rank;
    values[INDEX_TOTAL_VALUE] = 0;
    
    MPI_Win win;
    MPI_Win_create(values, 2 * sizeof(int), sizeof(int), MPI_INFO_NULL, MPI_COMM_WORLD, &win);
    MPI_Win_fence(MPI_MODE_NOPRECEDE | MPI_MODE_NOSTORE, win);
    for(int i=0; i<comm_size; i++)
    {
        MPI_Accumulate(&values[INDEX_MY_VALUE], 1, MPI_INT, i, INDEX_TOTAL_VALUE, 1, MPI_INT, MPI_SUM, win);
    }
    MPI_Win_fence(MPI_MODE_NOSUCCEED | MPI_MODE_NOPUT, win);
    MPI_Win_free(&win);

	// Verify integrity
	if(my_rank == 0)
	{
		int reference_value = 0;
		for(int i = 0; i < comm_size; i++)
		{
			reference_value += i;
		}

		if(reference_value != values[INDEX_TOTAL_VALUE])
		{
			printf("The value reduced (%d) differs from the reference (%d).\n", values[INDEX_TOTAL_VALUE], reference_value);
			MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
		}
	}

	printf("[MPI process %d] Value reduced = %d.\n", my_rank, values[INDEX_TOTAL_VALUE]);

	MPI_Finalize();

	return EXIT_SUCCESS;
}
