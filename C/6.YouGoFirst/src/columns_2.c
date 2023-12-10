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

	int values[10][comm_size];
	int my_values[10];
	for(int i = 0; i < 10; i++)
	{
		for(int j = 0; j < comm_size; j++)
		{
			values[i][j] = i * 10 + j;
		}
	}

	// Distribute columns across MPI processes
	MPI_Datatype column;
	MPI_Type_vector(10, 1, comm_size, MPI_INT, &column);
	MPI_Type_commit(&column);
	if(my_rank == 0)
	{
		for(int i = 0; i < comm_size; i++)
		{
			if(i == my_rank)
			{
				for(int j = 0; j < 10; j++)
				{
					my_values[j] = values[j][i];
				}
			}
			else
			{
				MPI_Send(&values[0][i], 1, column, i, 0, MPI_COMM_WORLD);
			}
		}
	}
	else
	{
		MPI_Recv(&my_values[0], 10, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}

	// Print the values held on each MPI process
	for(int i = 0; i < comm_size; i++)
	{
		if(i == my_rank)
		{
			for(int j = 0; j < 10; j++)
			{
				printf("%d ", my_values[j]);
			}
			printf("\n");
		}
		MPI_Barrier(MPI_COMM_WORLD);
	}

	MPI_Finalize();

	return EXIT_SUCCESS;
}
