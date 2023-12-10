/**
 * @file gputogpu.c
 * @brief This example is to check that the compilation process works.
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
#include <cuda.h>
#include <cuda_runtime.h>

void launch_increment(int* array_device, int SIZE);

/**
 * @brief Just prints a desperate hello world message.
 **/
int main(int argc, char* argv[])
{
	// Initialise the MPI program
	MPI_Init(&argc, &argv);

	int my_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	const int SIZE = 10;
	const int SIZE_IN_BYTES = SIZE * sizeof(int);
	int array_host[SIZE];
	int* array_device = NULL;
	cudaMalloc((void**)&array_device, SIZE_IN_BYTES);
	if(my_rank == 0)
	{
		for(int i = 0; i < SIZE; i++)
		{
			array_host[i] = i;
		}
		
		// Without GPU-aware MPI
		// cudaMemcpy(array_device, array_host, SIZE_IN_BYTES, cudaMemcpyHostToDevice);
		// launch_increment(array_device, SIZE);
		// cudaMemcpy(array_host, array_device, SIZE_IN_BYTES, cudaMemcpyDeviceToHost);
		// MPI_Send(array_host, SIZE, MPI_INT, 1, 00, MPI_COMM_WORLD);

		// With GPU-aware MPI
		cudaMemcpy(array_device, array_host, SIZE_IN_BYTES, cudaMemcpyHostToDevice);
		launch_increment(array_device, SIZE);
		MPI_Send(array_device, SIZE, MPI_INT, 1, 0, MPI_COMM_WORLD);
	}
	else if(my_rank == 1)
	{
		// Without GPU-aware MPI
		// MPI_Recv(array_host, SIZE, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		// cudaMemcpy(array_device, array_host, SIZE_IN_BYTES, cudaMemcpyHostToDevice);
		// launch_increment(array_device, SIZE);
		// cudaMemcpy(array_host, array_device, SIZE_IN_BYTES, cudaMemcpyDeviceToHost);

		// With GPU-aware MPI
		MPI_Recv(array_device, SIZE, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		launch_increment(array_device, SIZE);
		cudaMemcpy(array_host, array_device, SIZE_IN_BYTES, cudaMemcpyDeviceToHost);

		for(int i = 0; i < SIZE; i++)
		{
			printf("%d ", array_host[i]);
		}
		printf("\n");
	}

	// Finalize the MPI program
	MPI_Finalize();

	return EXIT_SUCCESS;
}
