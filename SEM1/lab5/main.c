// mpirun -np 4 -mca btl ^openib a.out

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mpi.h"

#define T 400
#define ROW 20
#define COL 20
#define dP 0.05


typedef struct Tuple {
   int x;
   int y;
   int z;
} tuple;


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

int enqueue(tuple q[ROW * COL], int qX, int qY, int t, int* qSize) {
   q[*qSize].x = qX;
   q[*qSize].y = qY;
   q[*qSize].z = t;
   *qSize = *qSize + 1;

}

tuple dequeue(tuple q[ROW * COL], int* qSize) {
   tuple ret = q[0];
   for (int i = 0; i < ROW * COL; i++) {
      q[i] = q[i + 1];
   }
   *qSize = *qSize - 1;
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
   int *sizePointer = &qSize;

   for (int i = 0; i < ROW * COL; i++) {
      queue[i].x = -1;
      queue[i].y = -1;
      queue[i].z = -1;
   }
   for (int i = 0; i < ROW; i++) {
      if (g[i][0] == '*') {
         enqueue(queue, i, 0, 0, sizePointer);

      }
   }
   tuple prev;
   while (qSize != 0) {
      tuple t = dequeue(queue, sizePointer);
      g[t.x][t.y] = ' ';

      if (t.x > 0) {
         if (g[t.x - 1][t.y] == 'T') {
            enqueue(queue, t.x - 1, t.y, t.z + 1, sizePointer);
            g[t.x - 1][t.y] = '*';
         }
      }
      if (t.x < ROW - 1) {
         if (g[t.x + 1][t.y] == 'T') {
            enqueue(queue, t.x + 1, t.y, t.z + 1, sizePointer);
            g[t.x + 1][t.y] = '*';
         }
      }
      if (t.y > 0) {
         if (g[t.x][t.y - 1] == 'T') {
            enqueue(queue, t.x, t.y - 1, t.z + 1, sizePointer);
            g[t.x][t.y - 1] = '*';
         }
      }
      if (t.y < COL - 1 ) {
         if (g[t.x][t.y + 1] == 'T') {
            enqueue(queue, t.x, t.y + 1, t.z + 1, sizePointer);
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
   clock_t start = clock();
   int        size    ; // same
   int        rank    ; // different
   //
   MPI_Status status  ;
   int        tag = 0 ; // same!
   FILE*      f;


   int        k , j  ;
   int        length = ((int)1 / dP) + 1;
   double solution[length];
   double* tmp;
   tmp = malloc(length * sizeof(*tmp));

   for (int s = 0; s < length; s++) {
      solution[s] = 0;
   }


   MPI_Init(      &argc          , &argv ) ;
   MPI_Comm_size( MPI_COMM_WORLD , &size ) ;
   MPI_Comm_rank( MPI_COMM_WORLD , &rank ) ;

   if ( rank == 0 )
   {
      for ( k = 1 ; k < size ; k++ )
      {
         MPI_Recv( tmp , length , MPI_DOUBLE , k , tag , MPI_COMM_WORLD , &status ) ;
         for(int s=0;s<length;s++){
            printf("%d, %lf", k, tmp[s]);
         }


         for (int s = 0; s < length; s++) {
            solution[s] += tmp[s];
         }
      }
      f = fopen("data.txt","w");
      int in = 0;
      for (double s = 0; s < 1.000000000001; s += dP) {
         solution[in] /= T;
         fprintf(f, "%lf\n",solution[in] );
         in++;
      }
      clock_t end = clock();
      double time = (double)(end-start) / CLOCKS_PER_SEC;
      printf("%lf\n",time);

   }
   else
   {
      srand(rank);
      double step = 0.0;
      int index = 0;
      double i;

      //
      j = T / size ; // trials = 100 million
      //
      double* solution;
      solution = malloc(length * sizeof(*solution));

      char grid[ROW][COL];
      for (i = 0; i < 1.000000000001; i += dP) {
         for ( k = 0 ; k < j ; k++ )
         {
            fill(grid, i);
            step += (double)(startFire(grid)) / ROW;
         }
         solution[index] = step;
         index++;
      }

      MPI_Send( solution , length , MPI_DOUBLE , 0 , tag , MPI_COMM_WORLD ) ;
   }
   MPI_Finalize() ;
   return 0;
}
