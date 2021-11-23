#include <stdio.h>
#include <mpi.h>
#include <cmath>
#include <cstdlib>
#include <vector>

int main(int argc, char* argv[]) {

	int p, q, my_rank, proc, proc2, sum;
	int test = 0;

	MPI_Group group_world1, group_world2;
	MPI_Group comm1_group, comm2_group;

	MPI_Comm  comm1, comm2;

	std::vector<int> comm1_procs, comm2_procs;
	int my_comm1_rank = -1, my_comm2_rank = -1;
	int i = 0, err;

	MPI_Init(&argc, &argv);

	// Get the size of the mpi world
	MPI_Comm_size(MPI_COMM_WORLD, &p);

	// Get the rank of the local process
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	/* Enable default MPI error handler */
	MPI_Errhandler_set(MPI_COMM_WORLD, MPI_ERRORS_RETURN);

	q = std::sqrt(p);
	if ((q * q) != p) {
		if (my_rank == 0)
			printf("Error: p=%d must be a square number\n", p);
		MPI_Finalize();
		std::exit(EXIT_FAILURE);
	}

	// Make a communicator group
	for (int iproc(0); iproc < q; ++iproc)
		comm1_procs.push_back(iproc);

	MPI_Comm_group(MPI_COMM_WORLD, &group_world1);

	// Create the new group 
	MPI_Group_incl(group_world1, q, comm1_procs.data(), &comm1_group);

	// Create a communicator 
	err = MPI_Comm_create(MPI_COMM_WORLD, comm1_group, &comm1);
	if (err != MPI_SUCCESS) {
		printf("ERROR creating comm1\n");
		MPI_Finalize();
		std::exit(EXIT_FAILURE);
	}

	// Get rank in group comm1_group
	err = MPI_Comm_rank(comm1, &my_comm1_rank);
	if (err != MPI_SUCCESS) {
		printf("P[%d] is not a member of comm1\n", my_rank);
	}
	else {
		printf("P[%d] is a member of comm1 group: PCM1[%d]\n", my_rank, my_comm1_rank);
	}

	if (my_rank < q) {
		MPI_Comm_rank(comm1, &my_comm1_rank);
		test = 1;
		MPI_Bcast(&test, 1, MPI_INT, 0, comm1);
	}

	if (my_rank < q) {
      printf("BCast result> PW1[%d]/%d: PCM1[%d], bcast=[%d]\n", my_rank, q, my_comm1_rank, test);
	}


	MPI_Reduce(&test, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	if (my_rank < q)
		MPI_Bcast(&sum, 1, MPI_INT, 0, comm1);

	printf("PW1[%d]/%d: PCM1[%d], sum=[%d], sum+my_comm1_rank = %d\n", my_rank, q, my_comm1_rank, sum, sum + my_comm1_rank);

	MPI_Finalize();

}