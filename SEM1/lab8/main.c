//
// Torbert, 10 October 2019
//
#include <stdio.h>
#include <math.h>
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
#define n (int)( 0.5 + ( 90 * 60 * 60 ) / DT )


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

int main(int argc, char** argv)
{
	if (argc != 2) { fprintf(stdout, "idiot\n"); return -1; }
	//
	//////////////////////////////////////////////////
	//
	FILE*  fout  ;
	//
	int    j     ;
	//
	double a = 0, a_moon = 0;
	double r = 3.844e8 ; // distance from Earth

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
	t[0]  =          96302.0 ;

	double theta;
	sscanf(argv[1],"%lf",&theta);
	theta *= 3.1415 / 180;

	// Reserved for spaceship
	x[0]  =          (R + 202751774.4) * cos(theta) ;
	y[0]  =          (R + 202751774.4) * sin(theta)   ;
	vx[0] = 		 1527.048 * cos(theta) ;
	vy[0] =          1527.048 * sin(theta) ;
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
		a = -((G*M) / pow(d_moon[j],2));
		vx_moon[j] = vx_moon[j-1] + DT*a*(x_moon[j]/d_moon[j]);
		vy_moon[j] = vy_moon[j-1] + DT*a*(y_moon[j]/d_moon[j]);


		x[j] = x[j-1] + DT * vx[j-1];
		y[j] = y[j-1] + DT * vy[j-1];

		// Earth and spaceship
		d[j] = sqrt( pow(x[j],2) + pow(y[j], 2) );
		a = - ((G*M) / pow(d[j],2));

		// Spaceship and Moon
		double dim = pow(x_moon[j]-x[j], 2) + pow(y_moon[j]-y[j],2);
		double hyp = sqrt(dim);
		a_moon = - ((G*M_MOON)/dim);

		if(hyp < R_MOON){
			printf("Crashed into moon");
			return -1;
		}


		vx[j] = vx[j-1] + DT*a*(x[j]/d[j]) + DT*a_moon*((x_moon[j]-x[j])/hyp);
		vy[j] = vy[j-1] + DT*a*(y[j]/d[j]) + DT*a_moon*((y_moon[j]-y[j])/hyp);


	}
	//
	//////////////////////////////////////////////////
	//
	fout = fopen( "orbit.txt" , "w" ) ;
	//
	for( j = 0 ; j < n ; j += 200 )
	{
		fprintf( fout , "%d %0.16f %0.16f %0.16f %0.16f %0.16f %0.16f %0.16f %0.16f\n" , j , t[j], x[j] , y[j], d[j], vx[j], vy[j], x_moon[j], y_moon[j]) ;
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
