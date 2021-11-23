#include <stdio.h>
#include <mpi.h>
#include <cmath>

int main(int argc, char* argv[]) {
  int p, my_rank, reorder, my_cart_rank, ierr, nrows, ncols;
  const int ndims = 1;
  MPI_Comm comm1D;
  int dims[ndims], coord[ndims], wrap_around[ndims];

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &p);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  // Let MPI create a suitable number of subdivisions
  dims[0] = 0;
  //
  // Set number of subdivisions
  //dims[0] = 4;

  // We create a cartesian topology with p nodes, ndims dimensions and we set dims[0]
  // so that MPI creates a suitable subdivision with the available number of nodes 
  // in each dimension
  MPI_Dims_create(p, ndims, dims);
  if (my_rank == 0) {
    printf("PW[%d]/[%d]: PEdims = [%d] \n", my_rank, p, dims[0]);
  }
  fflush(stdout);
  MPI_Barrier(MPI_COMM_WORLD);

  // create cartesian mapping
  wrap_around[0] = 0;
  reorder = 1;
  ierr = 0;
  ierr = MPI_Cart_create(MPI_COMM_WORLD, ndims, dims, wrap_around, reorder, &comm1D);

  if (ierr != 0) printf("ERROR[%d] creating CART\n", ierr);

  MPI_Cart_coords(comm1D, my_rank, ndims, coord);

  /* find my coordinates in the cartesian communicator group */
  MPI_Cart_rank(comm1D, coord, &my_cart_rank);

  printf("PW[%d]: my_cart_rank PCM[%d], my coords = (%d)\n", my_rank, my_cart_rank, coord[0]);

  MPI_Comm_free(&comm1D);
   
  MPI_Finalize();

  return EXIT_SUCCESS;
}