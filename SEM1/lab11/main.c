#include <stdio.h>
#include <stdbool.h>

#define M 640
#define N 480
#define MAX 1000

int main(void)
{
   int rgb[N][M][3];
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
            if(x*x + y*y > 4){
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