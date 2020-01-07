//
// Torbert, 10 October 2019
//
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "mpi.h"
//
// https://physics.nist.gov/cgi-bin/cuu/Value?bg
//
#define G 6.674e-11 // (m^3)(kg^-1)(s^-2)
#define M 5.972e+24 // kg
#define R 6.371e+6  // m

#define M_MOON 7.349e+22 // kg
#define R_MOON 1.7374e+6 // m
#define V_MOON 1023.157  // m/s
//
// parameters
//
#define DT 0.25     // s
#define n (int)( 0.5 + ( 360 * 60 * 60 ) / DT )


double  t[n] ;
double  x[n] ;
double  y[n] ;
double vx[n] ;
double vy[n] ;
double d[n]  ;
double x_moon[n];
double y_moon[n];
double vx_moon[n];
double vy_moon[n];
double d_moon[n];

int main( int argc , char* argv[] )
{
   	clock_t start = clock();
   	int        size    ; // same
   	int        rank    ; // different
   	//
   	MPI_Status status  ;
   	int        tag = 0 ; // same!

	MPI_Init(      &argc          , &argv ) ;
   	MPI_Comm_size( MPI_COMM_WORLD , &size ) ;
   	MPI_Comm_rank( MPI_COMM_WORLD , &rank ) ;

	if(rank == 0){
		double theta = 180;
		double speed = 1527.048;
		{
			int count = 0;
			while(theta == 180){
				for ( int k = 1 ; k < size ; k++ )
				{
					double tmp = theta + (10*count);
					MPI_Send( &tmp, 1, MPI_DOUBLE, k, tag, MPI_COMM_WORLD);
					MPI_Send( &speed, 1, MPI_DOUBLE, k, tag, MPI_COMM_WORLD);
					MPI_Recv( &theta , 1 , MPI_DOUBLE , k , tag , MPI_COMM_WORLD , &status ) ;
					count ++;
				}
			}
		}
		{

		}
	}else{
		double theta, speed; 
		MPI_Recv(&theta, 1, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD, &status);
		MPI_Recv(&speed, 1, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD, &status);

		theta *= 3.141592653589793238462643383279502 / 180.0;

		x[0] = r + ( 2 * R_MOON ) * cos( theta ) ;
		y[0] = 0 + ( 2 * R_MOON ) * sin( theta ) ;

		vx[0] = 		 speed * cos(theta) ;
		vy[0] =          speed * sin(theta) ;
		d[0]  =			 sqrt(pow(x[0], 2) + pow(y[0],2)) ;


		x_moon[0]  = 		 r  ;
		y_moon[0]  = 		 0	;
		vx_moon[0] =		 0  ;
		vy_moon[0] = 		 V_MOON  ;
		d_moon[0]  =		 r  ;

		//
		//////////////////////////////////////////////////
		//
		for( j = 1 ; j < n ; j ++ ) 
		{
			t[j] = t[j-1] + DT           ;
			//
			x_moon[j] = x_moon[j-1] + DT * vx_moon[j-1] ;
			y_moon[j] = y_moon[j-1] + DT * vy_moon[j-1] ;
			d_moon[j] = sqrt( pow(x_moon[j],2) + pow(y_moon[j],2) );
			double a = -((G*M) / pow(d_moon[j],2));
			vx_moon[j] = vx_moon[j-1] + DT*a*(x_moon[j]/d_moon[j]);
			vy_moon[j] = vy_moon[j-1] + DT*a*(y_moon[j]/d_moon[j]);


			x[j] = x[j-1] + (DT * vx[j-1]);
			y[j] = y[j-1] + (DT * vy[j-1]);

			// Earth and spaceship
			d[j] = sqrt( pow(x[j],2) + pow(y[j], 2) );
			a = - ((G*M) / pow(d[j],2));

			// Spaceship and Moon
			double dim = pow(x_moon[j]-x[j], 2) + pow(y_moon[j]-y[j],2);
			double hyp = sqrt(dim);
			double a_moon = - ((G*M_MOON)/dim);


			vx[j] = vx[j-1] + DT*a*(x[j]/d[j]) + DT*a_moon*((x[j] - x_moon[j])/hyp);
			vy[j] = vy[j-1] + DT*a*(y[j]/d[j]) + DT*a_moon*((y[j] - y_moon[j])/hyp);


		}

	}

}
//
// end of file
//
