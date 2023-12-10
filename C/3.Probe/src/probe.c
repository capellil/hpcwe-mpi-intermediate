/**
 * @file probe.c
 * @brief This example is to illustrate how to receive a message of unknown
 * length without requiring an extra message sending the length.
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
 * @brief This application sends a message whose length is determined only at
 * runtime.
 * @details The sender will first communicate the length of the message to come,
 * so that the receive can allocate a buffer accordingly, before sending the
 * actual data.
 **/
int main(int argc, char* argv[])
{
	MPI_Init(&argc, &argv);

	int my_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	int comm_size;
	MPI_Comm_size(MPI_COMM_WORLD, &comm_size);

	if(my_rank == 0)
	{
		// Send the count
		int count = rand() % 100;
		MPI_Send(&count, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);

		// Send the data
		int array[count];
		for(int i = 0; i < count; i++)
		{
			array[i] = i;
		}
		MPI_Send(array, count, MPI_INT, 1, 0, MPI_COMM_WORLD);
	}
	else if(my_rank == 1)
	{
		// Receive the length
		int count;
		MPI_Recv(&count, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

		// Receive the data
		int* array = (int*)malloc(count * sizeof(int));
		MPI_Recv(array, count, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		for(int i = 0; i < count; i++)
		{
			if(array[i] != i)
			{
				printf("Value %d received is %d, instead of %d.\n", i, array[i], i);
				free(array);
				MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
			}
		}
		printf("All %d elements received and correct.\n", count);
		free(array);
	}

	MPI_Finalize();

	return EXIT_SUCCESS;
}
