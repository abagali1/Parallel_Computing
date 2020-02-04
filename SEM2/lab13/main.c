#include <stdio.h>

#define M 1920
#define N 1080

typedef struct
{
   double x ;
   double y ;
   double z ;
} triple ;

typedef struct
{
    int r;
    int g;
    int b;
} Color;

typedef struct 
{
    double r;
    Color h;
    triple c
} Sphere;


triple e = { 0.50 , 0.50 , -1.00 } ; // the eye
triple g = { 0.00 , 1.25 , -0.50 } ; // the light

double dotp( triple t , triple u )
{
   return t.x * u.x + t.y * u.y + t.z * u.z ;
}

void diff( triple* t , triple u , triple v ) // t = u - v
{
   t->x = u.x - v.x ;
   t->y = u.y - v.y ;
   t->z = u.z - v.z ;
}

void init(Sphere* a)
{
   a[0].c.x =      0.50 ;
   a[0].c.y = -20000.00 ; // the floor
   a[0].c.z =      0.50 ;
   a[0].r   =  20000.25 ;
   a[0].h.r =    205    ; // color is Peru
   a[0].h.g =    133    ;
   a[0].h.b =     63    ;
   //
   a[1].c.x =      0.50 ;
   a[1].c.y =      0.50 ;
   a[1].c.z =      0.50 ;
   a[1].r   =      0.25 ;
   a[1].h.r =      0    ; // color is Blue
   a[1].h.g =      0    ;
   a[1].h.b =    255    ;
   //
   a[2].c.x =      1.00 ;
   a[2].c.y =      0.50 ;
   a[2].c.z =      1.00 ;
   a[2].r   =      0.25 ;
   a[2].h.r =      0    ; // color is Green
   a[2].h.g =    255    ;
   a[2].h.b =      0    ;
   //
   a[3].c.x =      0.00 ;
   a[3].c.y =      0.75 ;
   a[3].c.z =      1.25 ;
   a[3].r   =      0.50 ;
   a[3].h.r =    255    ; // color is Red
   a[3].h.g =      0    ;
   a[3].h.b =      0    ;
}


int main(void){
    FILE* fout;
    Color** rgb = malloc(sizeof(Color*) * N);

    for (int i = 0; i < N; i++)
    {
        rgb[i] = malloc(sizeof(Color) * M);
    }
   Sphere spheres[4];
   init(spheres);

    double aspect_ratio = (double)M/(double)N;
    int Px, Py;
    double x,y;
    for(Py = 0; Py < N; Py++){
        for(Px = 0; Px < M; Px++){
        }
    }




   fout = fopen( "ms.ppm" , "w" ) ;
   fprintf( fout , "P3\n" ) ;
   fprintf( fout , "%d %d\n" , M , N ) ;
   fprintf( fout , "255\n" ) ;
   for(int Py = 0 ; Py < N ; Py++ )
   {
      for(int Px = 0 ; Px < M ; Px++)
      {
         fprintf( fout , "%d %d %d\n" ,
          rgb[Py][Px].r , rgb[Py][Px].g , rgb[Py][Px].b ) ;
      }
   }
   fclose( fout ) ;
   return 0 ;
}