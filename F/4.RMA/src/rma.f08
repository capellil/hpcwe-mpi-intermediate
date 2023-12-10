!> @file rma.f08
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
	INTEGER :: value
	INTEGER :: reduced_value
	INTEGER :: reference_value
	INTEGER :: i

	CALL MPI_Init()

	! Get basic information
	CALL MPI_Comm_rank(MPI_COMM_WORLD, my_rank)
	CALL MPI_Comm_size(MPI_COMM_WORLD, comm_size)

	! Perform the all reduce.
	value = my_rank
	CALL MPI_Allreduce(value, reduced_value, 1, MPI_INTEGER, MPI_SUM, MPI_COMM_WORLD)

	! Verify integrity
	IF (my_rank == 0) THEN
		reference_value = 0
		DO i = 0, comm_size -1
			reference_value = reference_value + i
		END DO

		IF (reference_value .NE. reduced_value) THEN
			WRITE(*, '(A,I0,A,I0,A)') 'The value reduced (', reduced_value, ') differs from the reference (', reference_value, '.'
			CALL MPI_Abort(MPI_COMM_WORLD, -1)
		END IF
	END IF

	WRITE(*, '(A,I0,A,I0,A)') '[MPI process ', my_rank, '] Value reduced = ', reduced_value

	CALL MPI_Finalize()
END PROGRAM main