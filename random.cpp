#include <cmath>

#include "random.h"

/*
class random{

	float buff[55];
	int ip,jp,kp;
public: random();
		random(int);
		~random();
		float value();
		float gaussian();
		void sphere(float *, float *, float *);
private: void initialize_buffer(int);
};
*/

random::random()
{	initialize_buffer(10101);}

random::random(int seed)
{	initialize_buffer(seed);}

random::~random(){}

//
// sphere uses the rejection method to generate points within a
// unit sphere.
// it then maps them to the surface
// this gives a uniform distribution of orientation vectors.
//
void random::sphere( float* x, float* y, float* z)
{
	float rx,ry,rz,r;
	r = 100.;
	while( r > 1.)
	{
		rx = (float)2.*value()-(float)1.;
		ry = (float)2.*value()-(float)1.;
		rz = (float)2.*value()-(float)1.;
		r = rx*rx +ry*ry + rz*rz;
		if( r < 1.e-7) r = 100.;
	}	
//	r = (float)1./r;
//	*x = rx*r;
//	*y = ry*r;
//	*z = rz*r;
}
//
// was originally going to be a sphere
//  but in high dimensions the spherical recjection is abysimaly bad
//  since there is another rejection step, a box is better
//
void random::nbox( int size, float *values)
{
	for( int i=0; i< size;i++)
	values[i] = (float)2.*value() -(float)1.;
}

float random::cauchy()
{
	float x;
	x = value();
	if( x < 0.000001) x = 0.000001;
	if( x > 1.0) x = 1.0;
	if( value() < 0.5)
	return sqrt( 1./x-1.);
	else
	return -sqrt( 1./x-1.);
}
//
// find a uniformly distributed point in the unit circle
// then transform it into a guassian point.
// one could generate two gaussians using x1,x2
// but they would not be fully independent.
//
float random::gaussian()
{
	
	float x1,x2,norm;
	
	norm = 2.;
	while( norm > 1.)
	{
		x1 = (float)2.*value()-(float)1.; x2 = (float)2.*value()-(float)1.;
		norm = x1*x1 + x2*x2;
	}

	if( norm < 1.e-9) norm = (float)1.e-9;
	return x1*(float)sqrt( -2.*log(norm)/norm );

}


void random::initialize_buffer(int seed)
	{
	for( ip=0; ip < 55; ip++)
	    { seed = (seed*2349+14867)%32767;
		buff[ip] = (float)seed/(float)32767.;
		if( buff[ip] > 1.) buff[ip] = buff[ip]-(float)1.;
		if( buff[ip] < 0.) buff[ip] = buff[ip]+(float)1.;
		}
/*	ip = 24; jp=55-ip; kp = 0; */
	ip =24; jp=54; kp=0; 
	ip = 23; jp = 24; kp = 0;
	}

	
	
float random::value()
{
	int i;
	float xva;
	i = kp;
	xva = buff[jp]+buff[ip];
	if( xva > 1.) xva = xva -(float)1.;
	buff[kp] = xva;
	kp = (kp+1)%55;
	ip = (ip+1)%55;
	jp = (jp+1)%55;
	return buff[i];
}
