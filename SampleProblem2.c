#include <stdio.h>
#include "mpi.h"

#define Host_ID   0

int main(int argc, char *argv[])
{
  int N = 2;
  int i;

  int myid, numprocs;

  int a = 0;

  MPI_Init( &argc, &argv );

  MPI_Comm_size( MPI_COMM_WORLD, &numprocs );
  MPI_Comm_rank( MPI_COMM_WORLD, &myid );

  printf("myid = %d of %d\n", myid, numprocs);


  /* Create size N group     */
  MPI_Group MPI_GROUP_WORLD, group1; 
  MPI_Comm_group( MPI_COMM_WORLD, &MPI_GROUP_WORLD ); 
  int ranks[N];
  for ( i = 0; i < N; i++ )
    ranks[i] = i;
  MPI_Group_incl( MPI_GROUP_WORLD, N, ranks, &group1 ); 

  /* group -> comm */
  MPI_Comm comm1;
  MPI_Comm_create( MPI_COMM_WORLD, group1, &comm1);

  if ( myid == Host_ID )
    a = 1;

  MPI_Barrier(MPI_COMM_WORLD);

  /* Bcast in size N comm   */
  if ( myid < N )
    MPI_Bcast ( &a, 1, MPI_INT, Host_ID, comm1 );
  printf("myid = %d, a = %d\n", myid, a);

  MPI_Barrier(MPI_COMM_WORLD);

  if ( myid == Host_ID )
    a = 2;

  MPI_Barrier(MPI_COMM_WORLD);

  /* Bcast in MPI_COMM_WORLD */

  MPI_Bcast ( &a, 1, MPI_INT, Host_ID, MPI_COMM_WORLD );
  printf("myid = %d, a = %d\n", myid, a);


  MPI_Finalize();   
  return 0;
}