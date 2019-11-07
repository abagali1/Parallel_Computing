//
// Torbert, 10 October 2019
//
#include <stdio.h>
#include <math.h>
//
// https://physics.nist.gov/cgi-bin/cuu/Value?bg
//
#define G 6.674e-11 // (m^3)(kg^-1)(s^-2)
//
// https://ssd.jpl.nasa.gov/?planet_phys_par
//
#define M 7.349e+22 // kg
#define R 1.7374e+6 // m
#define V 1023.157  // m/s
//
// parameters
//
#define DT 0.25     // s
//
int main()
{
	//
	// time intervals - duration is 90 minutes
	//
	int n = (int)( 0.5 + ( 1.5 * 60 * 60 ) / DT ) ;
	//
	//////////////////////////////////////////////////
	//
	double  t[n] ;
	double  x[n] ;
	double  y[n] ;
	double vx[n] ;
	double vy[n] ;
	double d[n]  ;
	//
	//////////////////////////////////////////////////
	//
	FILE*  fout  ;
	//
	int    j     ;
	//
	double r , a ;
	//
	//////////////////////////////////////////////////
	//
	// International Space Station
	//
	// https://www.nasa.gov/sites/default/files/atoms/files/np-2015-05-022-jsc-iss-guide-2015-update-111015-508c.pdf
	//
	// Page 54 - altitude : 370 km to 460 km
	// Page 54 - speed    : 28,000 km per hour
	//
	t[0]  =          0.0 ;
	x[0]  =          0.0 ;
	y[0]  = R + 400000.0 ;
	vx[0] =       7670.1 * 1.5 ;
	vy[0] =          0.0 ;
	d[j]  =			 0.0 ;
	//
	//////////////////////////////////////////////////
	//
	for( j = 1 ; j < n ; j ++ )
	{
		t[j] = t[j-1] + DT           ;
		//
		x[j] = x[j-1] + DT * vx[j-1] ;
		y[j] = y[j-1] + DT * vy[j-1] ;
		d[j] = sqrt( (x[j]*x[j]) + (y[j]*y[j]) ); 

		//
		// calculate a
		//
		a = ((G*M) / (d[j]*d[j]));
		// update vx
		vx[j] = vx[j-1] + DT*a*(x[j]/d[j]);
		// update vy
		vy[j] = vy[j-1] + DT*a*(y[j]/d[j]);


	}
	//
	//////////////////////////////////////////////////
	//
	fout = fopen( "orbit.txt" , "w" ) ;
	//
	for( j = 0 ; j < n ; j ++ )
	{
		fprintf( fout , "%d %0.16f %0.16f %0.16f %0.16f %0.16f %0.16f\n" , j , t[j], x[j] , y[j], d[j], vx[j], vy[j] ) ;
		//
		// what else to print ?
		//
	}
	//
	fclose( fout ) ;
	//
	return 0 ;
}
//
// end of file
//