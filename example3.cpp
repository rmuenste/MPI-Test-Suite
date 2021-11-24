#include <stdio.h>
#include <mpi.h>
#include <cmath>

int main(int argc, char* argv[]) {
  int p, my_rank, reorder, my_cart_rank, ierr, nrows, ncols;
  const int ndims = 2;
  MPI_Comm comm2D;
  int dims[ndims], coord[ndims], wrap_around[ndims];

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &p);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  if (argc == 3) {
    nrows = std::atoi(argv[1]);
    ncols = std::atoi(argv[2]);
    dims[0] = nrows;
    dims[1] = ncols;
    if (nrows * ncols != p) {
      if (my_rank == 0)
      {
        printf("ERROR: nrows * ncols = %d * %d = %d != %d\n", nrows, ncols, nrows * ncols, p);
        fflush(stdout);
      }
      MPI_Finalize();
      std::exit(EXIT_FAILURE);
    }
    if (my_rank == 0)
    {
      printf("OK!: nrows * ncols = %d * %d = %d != %d\n", nrows, ncols, nrows * ncols, p);
      fflush(stdout);
    }
  }
  else {
    nrows = ncols = std::sqrt(p);
    dims[0] = dims[1] = 0;
  }

  MPI_Dims_create(p, ndims, dims);
  if (my_rank == 0)
    printf("PW[%d]/[%d]: PEdims = [%d x %d] \n", my_rank, p, dims[0], dims[1]);

  wrap_around[0] = wrap_around[1] = 0;
  reorder = 1;
  ierr = 0;
  ierr = MPI_Cart_create(MPI_COMM_WORLD, ndims, dims, wrap_around, reorder, &comm2D);

  if (ierr != 0) printf("ERROR[%d] creating CART\n", ierr);

  MPI_Cart_coords(comm2D, my_rank, ndims, coord);

  MPI_Cart_rank(comm2D, coord, &my_cart_rank);

  printf("PW[%d]: my_cart_rank PCM[%d], my coords = (%d,%d)\n", my_rank, my_cart_rank, coord[0], coord[1]);

  MPI_Comm_free(&comm2D);

  MPI_Finalize();
}
