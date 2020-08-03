#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mpi.h"
#define Host_ID       0   

int main(int argc, char *argv[]){
  int myid, numprocs;

  MPI_Init(&argc, &argv);   
  MPI_Comm_rank(MPI_COMM_WORLD, &myid);
  MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

  int N;
  N = 242;

  int iproc, Nproc, iprocINI, iprocEND;
  int Sumproc, Sum;
  if ( myid < N % numprocs ){
    Nproc  = N / numprocs + 1;
  }
  else{
    Nproc  = N / numprocs;
  }
  iprocINI = ceil( myid * (double) N / numprocs );
  iprocEND = iprocINI + Nproc;

  double* procList;
  procList = (double*)malloc( sizeof(double) * Nproc );
  int procListindex = 0;

  for ( iproc = iprocINI; iproc < iprocEND; iproc++){
    srand((myid+iproc)*(myid+1000));
    double RandomNum = (double) rand() / RAND_MAX;
    //printf("myid=%d: %f\n",myid,RandomNum);
    if ( RandomNum > 0.4 ){
      procList[procListindex] = RandomNum;
      procListindex++;
    }
  }

  printf("myid = %d: procListNum = %d\n", myid, procListindex);

  /* Gatherv */
  MPI_Barrier(MPI_COMM_WORLD);

  MPI_Finalize();
  return 0;
}