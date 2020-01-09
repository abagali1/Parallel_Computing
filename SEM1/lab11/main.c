#include <stdio.h>

#define M 640
#define N 480
#define MAX 1000

int main(void)
{
   int rgb[N][M][3];
   int y , x;
   FILE* fout;
   double palette[MAX];

   for( y = 0 ; y < N ; y++ )
   {
      for( x = 0 ; x < M ; x++)
      {  
         double x0 = (x*4/M)-2.0;
         double y0 = (y*3/N)-1.5;
         double x = 0;
         double y = 0;
         int i = 0;
         while(x*x + y*y <= 4 && i < MAX){
            double temp = x*x - y*y + x0;
            y = 2*x*x + y0;
            x = temp;
            i++;
         }
      }
   }
   fout = fopen( "ms.ppm" , "w" ) ;
   fprintf( fout , "P3\n" ) ;
   fprintf( fout , "%d %d\n" , M , N ) ;
   fprintf( fout , "255\n" ) ;
   for( y = 0 ; y < N ; y++ )
   {
      for( x = 0 ; x < M ; x++)
      {
         fprintf( fout , "%d %d %d\n" ,
          rgb[y][x][0] , rgb[y][x][1] , rgb[y][x][2] ) ;
      }
   }
   fclose( fout ) ;
   return 0 ;
}