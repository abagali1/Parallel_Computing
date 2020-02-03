#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"


#define M 1920
#define N 1080
#define MAX 100000


int mandelbrot(int Px, int Py){
      double x0 = (Px*1.2087219238309999/M)-0.167883300781;
      double y0 = (Py*1.2084777832009999/N)-0.166906738281;
      double x,y = 0;
      int i = 0;
      while(i<MAX){
         if(x*x + y*y > 20){
            return 0;
         }
         double temp = x*x - y*y + x0;
         y = 2*x*y + y0;
         x = temp;
         i++;
      }
      return i;
}


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
