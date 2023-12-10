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

    if(comm_size != 16)
	{
		printf("This application is meant to be run with 16 MPI processes, not %d.\n", comm_size);
	}

	int dims[2] = {4, 4};
	int periods[2] = {0, 0};
	int reorder = 0;
	MPI_Comm cart_comm;
	MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, reorder, &cart_comm);
	int up;
	int down;
	int right;
	int left;
	MPI_Cart_shift(cart_comm, 0, 1, &down, &up);
	MPI_Cart_shift(cart_comm, 1, 1, &right, &left);
	MPI_Request requests[4];
	int value_up = 0;
	MPI_Irecv(&value_up, 1, MPI_INT, up, 0, MPI_COMM_WORLD, &requests[0]);
	int value_down = 0;
	MPI_Irecv(&value_down, 1, MPI_INT, down, 0, MPI_COMM_WORLD, &requests[1]);
	int value_right = 0;
	MPI_Irecv(&value_right, 1, MPI_INT, right, 0, MPI_COMM_WORLD, &requests[2]);
	int value_left = 0;
	MPI_Irecv(&value_left, 1, MPI_INT, left, 0, MPI_COMM_WORLD, &requests[3]);
	MPI_Send(&my_rank, 1, MPI_INT, up, 0, MPI_COMM_WORLD);
	MPI_Send(&my_rank, 1, MPI_INT, down, 0, MPI_COMM_WORLD);
	MPI_Send(&my_rank, 1, MPI_INT, right, 0, MPI_COMM_WORLD);
	MPI_Send(&my_rank, 1, MPI_INT, left, 0, MPI_COMM_WORLD);
	MPI_Waitall(4, requests, MPI_STATUSES_IGNORE);
	int total = value_up + value_down + value_right + value_left;
	printf("[MPI process %d] Sum around me = %d.\n", my_rank, total);

	MPI_Finalize();

	return EXIT_SUCCESS;
}
