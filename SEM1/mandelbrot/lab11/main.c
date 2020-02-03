#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define M 1920
#define N 1080
#define MAX 10000

int main(void)
{
   // int rgb[N][M][3];
   int*** rgb = malloc(sizeof(int**) * N);

   for (int i = 0; i < N; i++)
   {
      rgb[i] = malloc(sizeof(int*) * M);

      for (int j = 0; j < M; j++)
      {
         rgb[i][j] = malloc(sizeof(int) * 3);
      }
   }

   int Py , Px;
   FILE* fout;
   double palette[MAX];

   for( Py = 0 ; Py < N ; Py++ )
   {
      for( Px = 0 ; Px < M ; Px++)
      {  
         double x0 = (Px*4.0/M)-2.0;
         double y0 = (Py*4.0/N)-2.0;
         double x = 0;
         double y = 0;
         bool broke = true;
         int i = 0;
         while(i < MAX){
            if(x*x + y*y > 20){
               broke = false;
               break;
            }
            double temp = x*x - y*y + x0;
            y = 2*x*y + y0;
            x = temp;
            i++;
         }
         if(broke){
            rgb[Py][Px][0] = 0;
            rgb[Py][Px][1] = 0;
            rgb[Py][Px][2] = 255;
         }else{
            rgb[Py][Px][0] = 0;
            rgb[Py][Px][1] = 0;
            rgb[Py][Px][2] = 0;
         }
      }
   }
   fout = fopen( "ms.ppm" , "w" ) ;
   fprintf( fout , "P3\n" ) ;
   fprintf( fout , "%d %d\n" , M , N ) ;
   fprintf( fout , "255\n" ) ;
   for( Py = 0 ; Py < N ; Py++ )
   {
      for( Px = 0 ; Px < M ; Px++)
      {
         fprintf( fout , "%d %d %d\n" ,
          rgb[Py][Px][0] , rgb[Py][Px][1] , rgb[Py][Px][2] ) ;
      }
   }
   fclose( fout ) ;
   return 0 ;
}