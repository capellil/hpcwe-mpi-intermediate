!> @file preamble.f08
!> @brief This example is to check that the compilation process works.
!> @details You will find two folders in this example, and will in all examples
!> provided in this session:
!> - src: contains the source code.
!> - bin: contains the binary produced.
!> The makefile provided already sets everything up for you:
!> - To compile: `make`.
!> - To execute: `sbatch submission.slurm`.
!> If you have any questions, do not hesitate.
!> @author Ludovic Capelli (l.capelli@epcc.ed.ac.uk)

!> @brief Just prints a desperate hello world message.
PROGRAM main
	USE mpi_f08

	IMPLICIT NONE

	! Initialise the MPI program
	WRITE(*, '(A)') 'Hello world, I know nothing about other MPI processes yet :('
	! Finalize the MPI program
END PROGRAM main