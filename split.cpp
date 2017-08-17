// simple program to split a kdd format file.
// for generating nfolds.
// rng is controlled by a seed so you can get reproducible and different
// splits.

//#ifndef RANDOM_RNG_INCLUDED
//#define RANDOM_RNG_INCLUDED
#include <cmath>
class Random {

	float buff[55];
	int ip,jp,kp;
public: Random();
		Random(int);
		~Random();
		float value();
		float gaussian();
		float cauchy();
		void sphere(float *, float *, float *);
		void nbox(int , float*);
private: void initialize_buffer(int);
};
//#endif



Random::Random()
{	initialize_buffer(10101);}

Random::Random(int seed)
{	initialize_buffer(seed);}

Random::~Random(){}

//
// sphere uses the rejection method to generate points within a
// unit sphere.
// it then maps them to the surface
// this gives a uniform distribution of orientation vectors.
//
void Random::sphere( float* x, float* y, float* z)
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
void Random::nbox( int size, float *values)
{
	for( int i=0; i< size;i++)
	values[i] = (float)2.*value() -(float)1.;
}

float Random::cauchy()
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
float Random::gaussian()
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


void Random::initialize_buffer(int seed)
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

	
	
float Random::value()
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


#include <cstdio>
#include <cstdlib>

using namespace std;
int main( int argc, char **argv)
{

if( argc < 6){
	fprintf(stderr,"USAGE split fraction seed in  out1 out2\n");
	exit(0);
	}
char line[10000];
float split;
int seed;
sscanf(argv[1],"%f",&split);
if( split < 0.0001F) split = 0.1F;
sscanf(argv[2],"%d",&seed); seed = abs(seed);
FILE *ip, *op1, *op2;
sscanf(argv[3],"%s",line);
ip = fopen(line,"r");
sscanf(argv[4],"%s",line);
op1 = fopen(line,"w");
sscanf(argv[5],"%s",line);
op2 = fopen(line,"w");

// deal with the header
int im;
fgets(line, sizeof(line),ip);
sscanf(line,"%d", &im);
fprintf(op1,"%s",line);
fprintf(op2,"%s",line);
for( int i=0; i< im; i++)
{
	fgets(line, sizeof(line),ip);
	fprintf(op1,"%s",line);
	fprintf(op2,"%s",line);
}
fgets(line, sizeof(line),ip);
fprintf(op1,"%s",line);
fprintf(op2,"%s",line);
//fgets(line, sizeof(line),ip);
//fprintf(op1,"%s",line);
//fprintf(op2,"%s",line);
// header should be echoed to both files.
Random *rng = new Random(seed);
while( fgets(line, sizeof(line), ip) == line)
{
	float x;
	x = rng->value();
	if( x < split)
		fprintf(op1,"%s",line);
	else
		fprintf(op2,"%s",line);
}// end of while;

fclose(ip);
fclose(op1);
fclose(op2);
}// end of main




