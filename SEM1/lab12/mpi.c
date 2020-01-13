#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "mpi.h"


#define M 1920
#define N 1080


int main( int argc , char* argv[] )
{
   int        size;
   int        rank;
   MPI_Status status;
   int        tag = 0;
   int        k;
   int* result = malloc(sizeof(int*) * M * N * 3);

   MPI_Init(      &argc          , &argv );
   MPI_Comm_size( MPI_COMM_WORLD , &size );
   MPI_Comm_rank( MPI_COMM_WORLD , &rank );

   srand( rank );

   if( rank == 0 )
   {
      for( k = 1 ; k < size ; k++ )
      {
         MPI_Recv( &result , 1 , MPI_DOUBLE , MPI_ANY_SOURCE , tag , MPI_COMM_WORLD , &status);
      }
   }
   else
   {

   }

   MPI_Finalize();
   return 0;
}
// 
// end of file
// 
