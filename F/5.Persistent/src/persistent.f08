!> @file preamble.f08
!> @brief This example is to practice with one-sided communications.
!> @details You will find two folders in this example, and will in all examples
!> provided in this session:
!> - src: contains the source code.
!> - bin: contains the binary produced.
!> The makefile provided already sets everything up for you:
!> - To compile: `make`.
!> - To execute: `sbatch submission.slurm`.
!> If you have any questions, do not hesitate.
!> @author Ludovic Capelli (l.capelli@epcc.ed.ac.uk)

!> @brief This implements an MPI all reduction using MPI_Allreduce. The 
!> objective is to rewrite it using one-sided communications instead.
PROGRAM main
	USE mpi_f08

	IMPLICIT NONE

	INTEGER :: my_rank
	INTEGER :: comm_size
	INTEGER :: count = 0
	INTEGER :: total = 0
	INTEGER :: i

	CALL MPI_Init()

	! Get basic information
	CALL MPI_Comm_rank(MPI_COMM_WORLD, my_rank)
	CALL MPI_Comm_size(MPI_COMM_WORLD, comm_size)

	DO i = 0, 999999
		CALL MPI_Reduce(my_rank, total, 1, MPI_INTEGER, MPI_SUM, 0, MPI_COMM_WORLD)
		count = count + 1
	END DO
	WRITE(*, '(A,I0,A,I0,A)') '[MPI process ', my_rank, '] ', count ,' rounds done.'

	CALL MPI_Finalize()
END PROGRAM main