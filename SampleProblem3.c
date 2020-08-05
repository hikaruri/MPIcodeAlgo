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

  int N = 242;

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

  int* procListNum;
  procListNum = (int*)malloc( sizeof(int) * numprocs );

  int procListindex = 0;

  /* 条件を満たす値だけを持つリストを生成 */
  for ( iproc = iprocINI; iproc < iprocEND; iproc++){
    srand((myid+iproc)*(myid+1000));
    double RandomNum = (double) rand() / RAND_MAX;
    if ( RandomNum > 0.9 ){
      procList[procListindex] = RandomNum;
      procListindex++;
    }
  }

  printf("myid:%d, ListNum=%d\n", myid, procListindex);

  double* procListGather;
  procListGather = (double*)malloc( sizeof(double) * procListindex );
  for ( int i = 0; i < procListindex; i++ ) procListGather[i] = procList[i];

  MPI_Barrier(MPI_COMM_WORLD);

  int ListCount = 0;
  
  MPI_Gather( &procListindex, 1, MPI_INT,
    procListNum, 1, MPI_INT, Host_ID, MPI_COMM_WORLD);

  MPI_Reduce( &procListindex, &ListCount, 1, MPI_INT, MPI_SUM,
          Host_ID, MPI_COMM_WORLD);
  
  double *List;
  List = (double*)malloc( sizeof(double) * ListCount );

  MPI_Barrier(MPI_COMM_WORLD);

  /* Gatherv */

  int* displs;
  displs = (int*)malloc( sizeof(int) * numprocs );
  displs[0] = 0;
  for ( int i = 1; i <= numprocs - 1; i++ ){
    displs[i] =  displs[i-1] + procListNum[i-1];
  }

  MPI_Gatherv( 
        procListGather, procListindex, MPI_DOUBLE,
        List,     procListNum, displs,
        MPI_DOUBLE, Host_ID, MPI_COMM_WORLD);
  
  if ( myid == Host_ID ){
    for ( int i = 0; i < ListCount; i++ )
      printf("List[%d] = %f\n", i, List[i]);
  }

  
  free(procList);
  free(procListNum);
  free(displs);
  free(List);

  MPI_Finalize();
  return 0;
}