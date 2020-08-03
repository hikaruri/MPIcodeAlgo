#include <stdio.h>
#include <math.h>
#include "mpi.h"
#define Host_ID 0   

int main(int argc, char *argv[]){

  int myid, numprocs;           

  MPI_Init(&argc, &argv);   

  /* ランク数の取得 */
  MPI_Comm_rank(MPI_COMM_WORLD, &myid);
  /* 全プロセス数の取得 */ 
  MPI_Comm_size(MPI_COMM_WORLD, &numprocs);   

  int N, M;
  N = 17;
  M = 11;

  int iproc, Nproc, iprocINI, iprocEND;
  int i,j;
  int Sumproc, Sum;
  int Summesh = N * M;

  /* 各プロセスの繰り返し数  */
  if ( myid < Summesh % numprocs ){
    Nproc = Summesh / numprocs + 1;
  }
  else{
    Nproc = Summesh / numprocs;
  }
  iprocINI = ceil( myid * (double) Summesh / numprocs );
  iprocEND = iprocINI + Nproc;

  printf("myid:%d: iprocINI=%d, iprocEND=%d\n", myid, iprocINI, iprocEND);

  /* シグマの並列計算 */
  Sumproc = 0; Sum = 0;

  for ( iproc = iprocINI; iproc < iprocEND; iproc++){
    i =  iproc       % N ;
    j = (iproc - i)  / N ;
    Sumproc += ( i + 1 ) * ( j + 1 );
  }

  MPI_Barrier(MPI_COMM_WORLD);

  MPI_Reduce( &Sumproc, &Sum, 1, MPI_INT, MPI_SUM, Host_ID, MPI_COMM_WORLD);

  MPI_Barrier(MPI_COMM_WORLD);

  if (myid == Host_ID) printf("Sum = %d\n", Sum);

  MPI_Finalize();   /* MPIの終了処理 */
  return 0;
}
