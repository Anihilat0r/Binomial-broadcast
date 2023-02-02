/*
	Broadcast algorithm for distributed memory computer using MPI standard's send and receive operations.
	In this case a binomial tree structure is used to define the communication paths between senders and
	receivers.
*/

int binomial_bcast(void* data, int count, MPI_Datatype datatype, int root, MPI_Comm communicator)
{
	int order, total_order;
	int relative_rank, snder_rank, recver_rank;

	//Use the relative ranks to form the tree and decide who communicates with who
	//Must use the actual ranks in the send and receive processes
	if (processor_rank >= root)
	{
		relative_rank = processor_rank - root;
	}
	else
	{
		relative_rank = processor_rank - root + processor_count;
	}

	total_order = ceil(log2(processor_count));

	//All processes except root receive data once
	if (relative_rank != 0)
	{
		order = ceil(log2(relative_rank + 1));
		snder_rank = processor_rank - pow(2, (order - 1));
		if (snder_rank < 0)
		{
			snder_rank += processor_count;
		}
		MPI_Recv(data, count, datatype, snder_rank, TAG_MATRIX_PARTITION, communicator, MPI_STATUS_IGNORE);
		printf("Process %d will receive data from process %d\n", processor_rank, snder_rank);

	}
	else
	{
		order = 0;
	}

	//Send to the processes using the binomial tree
	for (order; order < total_order; order++)
	{
		if ((relative_rank + pow(2, (order))) < processor_count)
		{
			// The distance betwwen sender and receiver is the same for both relative and actual tree 
			recver_rank = processor_rank + pow(2, (order));
			if (recver_rank >= processor_count)
			{
				recver_rank -= processor_count;
			}

			MPI_Send(data, count, datatype, recver_rank, TAG_MATRIX_PARTITION, communicator);
			printf("Process %d sent data to process %d\n", processor_rank, recver_rank);

		}
	}
	return 0;
}
