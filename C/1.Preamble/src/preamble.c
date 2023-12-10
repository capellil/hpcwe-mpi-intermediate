/**
 * @file preamble.c
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

/**
 * @brief Just prints a desperate hello world message.
 **/
int main(int argc, char* argv[])
{
	// Initialise the MPI program
	printf("Hello world, I know nothing about other MPI processes yet :'(\n");
	// Finalize the MPI program

	return EXIT_SUCCESS;
}
