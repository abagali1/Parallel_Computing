// mpirun -np 4 -mca btl ^openib a.out

#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

#define T 100
#define ROW 20
#define dP 0.05
#define COL 20


double r() {
   return rand() * 1.0 / RAND_MAX;
}

void fill(char g[][COL], double p ) {
   int i, j;
   for (i = 0; i < ROW; i++) {
      for (j = 0; j < COL; j++) {
         if (r() < p) {
            g[i][j] = 'T';
         } else {
            g[i][j] = ' ';
         }
      }
   }
}

void enqueue(tuple q[ROW * COL], int qX, int qY, int t, int qSize) {
   q[qSize].x = qX;
   q[qSize].y = qY;
   q[qSize].z = t;
   qSize++;
}

tuple dequeue(tuple q[ROW * COL], int qSize) {
   tuple ret = q[0];
   qSize--;
   for (int i = 0; i < ROW * COL; i++) {
      q[i] = q[i + 1];
   }
   return ret;
}

int startFire(char g[][COL]) {
   int i, j;
   int burning = 0;


   for (i = 0; i < ROW; i++) {
      if (g[i][0] == 'T') {
         g[i][0] = '*';
      }
   }
   tuple queue[ROW * COL];

   int step = 0;
   int qSize = 0;
   int timeCounter;

   for (int i = 0; i < ROW * COL; i++) {
      queue[i].x = -1;
      queue[i].y = -1;
      queue[i].z = -1;
   }
   for (int i = 0; i < ROW; i++) {
      if (g[i][0] == '*') {
         timeCounter = 0;
         enqueue(queue, i, 0, timeCounter,qSize);

      }
   }
   tuple prev;
   while (qSize != 0) {
      tuple t = dequeue(queue,qSize);
      printf("T: (%d, %d, %d)",t.x,t.y,t.z);
      g[t.x][t.y] = ' ';

      if (t.x > 0) {
         if (g[t.x - 1][t.y] == 'T') {
            enqueue(queue, t.x - 1, t.y, t.z+1,qSize);
            g[t.x - 1][t.y] = '*';
         }
      }
      if (t.x < ROW - 1) {
         if (g[t.x + 1][t.y] == 'T') {
            enqueue(queue, t.x + 1, t.y, t.z+1,qSize);
            g[t.x + 1][t.y] = '*';
         }
      }
      if (t.y > 0) {
         if (g[t.x][t.y - 1] == 'T') {
            enqueue(queue, t.x, t.y - 1, t.z+1,qSize);
            g[t.x][t.y - 1] = '*';
         }
      }
      if (t.y < COL - 1 ) {
         if (g[t.x][t.y + 1] == 'T') {
            enqueue(queue, t.x, t.y + 1, t.z+1,qSize);
            g[t.x][t.y + 1] = '*';
         }
      }
      if (t.z > prev.z) {
         step++;
      }
      prev = t;
   }
   return step;
}



int main( int argc , char* argv[] )
{
   int        size    ; // same
   int        rank    ; // different
   //
   MPI_Status status  ;
   int        tag = 0 ; // same!

   int        k , j  ;
   double     result ;

   MPI_Init(      &argc          , &argv ) ;
   MPI_Comm_size( MPI_COMM_WORLD , &size ) ;
   MPI_Comm_rank( MPI_COMM_WORLD , &rank ) ;

   if ( rank == 0 )
   {
      printf( "\n" ) ;
      //
      for ( k = 1 ; k < size ; k++ )
      {
         MPI_Recv( &result , 1 , MPI_DOUBLE , MPI_ANY_SOURCE , tag , MPI_COMM_WORLD , &status ) ;
         //
         j = status.MPI_SOURCE ;
         //
         printf( "%d %d %20.16f\n" , j , size , result ) ;
      }
      //
      printf( "\n" );
   }
   //
   // workers have rank > 0
   //
   else
   {
      srand(rank);
      result = 0.0 ;
      //
      j = T / size ; // trials = 100 million
      //
      char grid[ROW][COL];
      for (double i = 0; i < 1.000000000001; i += dP) {
         for ( k = 0 ; k < j ; k++ )
         {
            fill(grid,i);
            result += (double)(startFire(grid)) / ROW;
         }
      }
      //
      result /= T;
      //
      MPI_Send( &result , 1 , MPI_DOUBLE , 0 , tag , MPI_COMM_WORLD ) ;
   }
   //
   // boilerplate
   //
   MPI_Finalize() ;
   //
   return 0;
}
//
// end of file
//
