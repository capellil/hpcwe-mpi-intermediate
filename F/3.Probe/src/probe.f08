!> @file probe.f08
!> @brief This example is to illustrate how to receive a message of unknown
!> length without requiring an extra message sending the length.
!> @details You will find two folders in this example, and will in all examples
!> provided in this session:
!> - src: contains the source code.
!> - bin: contains the binary produced.
!> The makefile provided already sets everything up for you:
!> - To compile: `make`.
!> - To execute: `sbatch submission.slurm`.
!> If you have any questions, do not hesitate.
!> @author Ludovic Capelli (l.capelli@epcc.ed.ac.uk)

!> @brief This application sends a message whose length is determined only at
!> runtime.
!> @details The sender will first communicate the length of the message to come,
!> so that the receive can allocate a buffer accordingly, before sending the
!> actual data.
PROGRAM main
	USE mpi_f08

	IMPLICIT NONE

	INTEGER :: my_rank
	INTEGER :: comm_size
	INTEGER :: count
	REAL :: rn
	INTEGER, DIMENSION(:), ALLOCATABLE :: array
	INTEGER :: i

	CALL MPI_Init()

	CALL MPI_Comm_rank(MPI_COMM_WORLD, my_rank)
	CALL MPI_Comm_size(MPI_COMM_WORLD, comm_size)

	IF (my_rank .EQ. 0) THEN
		! Send the count
		CALL RANDOM_NUMBER(rn)
		count = INT(rn * 100)
		CALL MPI_Send(count, 1, MPI_INTEGER, 1, 0, MPI_COMM_WORLD)

		! Send the data
		ALLOCATE(array(0:count-1))

		DO i = 0, count-1
			array(i) = i
		END DO
		CALL MPI_Send(array, count, MPI_INTEGER, 1, 0, MPI_COMM_WORLD)
	ELSEIF (my_rank .EQ. 1) THEN
		! Receive the length
		CALL MPI_Recv(count, 1, MPI_INTEGER, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

		! Receive the data
		ALLOCATE(array(0:count-1))
		CALL MPI_Recv(array, count, MPI_INTEGER, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		DO i = 0, count-1
			IF (array(i) .NE. i) THEN
				WRITE(*, '(A,I0,A,I0,A,I0,A)') 'Value ', i, ' received is ', array(i), ' instead of ', i
				DEALLOCATE(array)
				CALL MPI_Abort(MPI_COMM_WORLD, -1)
			END IF
		END DO
		WRITE(*, '(A,I0,A)') 'All ', count, ' elements received and correct.'
		DEALLOCATE(array);
	END IF

	CALL MPI_Finalize()
END PROGRAM main