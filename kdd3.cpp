// bitpack 0...1 goes to -1,-1,-1 .... 1,1,1

#include "kdd.h"
// class definitions for the file

// nothing special in the constructors
// so use the defaults


kdd_datum::kdd_datum( int mykind, int s, float *d)
{
	data = new float[s];
	id = mykind;
	for( int i=0; i< s; i++)
		data[i] = d[i];
	inme = s;
}
kdd_datum::~kdd_datum(){ delete[] data;}


bool kdd::get_next(int &id, int s, float *d)
{
	if( iterator >= the_data.size()) return false;
	s = the_data[iterator]->inme;
	for( int i=0; i< s; i++)
		d[i] = the_data[iterator]->data[i];
	id = the_data[iterator++]->id;
	return true;
}


bool kdd::rewind_me(){ iterator = 0;}
/*
here's a header - from glass.

10
@attribute A1	numerical
@attribute A2	numerical
@attribute A3	numerical
@attribute A4	numerical
@attribute A5	numerical
@attribute A6	numerical
@attribute A7	numerical
@attribute A8	numerical
@attribute A9	numerical
@attribute class category 1 2 3 4 5 6
@data
// the only one that's different in my set is heart which is mixed numerical
// and category
*/

bool kdd::load( FILE *ip)
{
	char buffer[10000];
	int nfield;
// the header information.
	if( fgets(buffer, sizeof(buffer), ip) != buffer) return false;
	sscanf( buffer,"%d",&nfield);
	nfeature = nfield -1;
	for( int i=0; i< nfeature; i++)
	{
		if( fgets(buffer, sizeof(buffer), ip) != buffer) return false;
// can add parsing here for more complicated data sets
		if( buffer[0] != '@') return false; // it's an error
	}
// @attribute class category a b c d e ...
	if( fgets(buffer, sizeof(buffer), ip) != buffer) return false;
	if( buffer[0] != '@') return false; // it's an error
	char *cp;
	cp = &buffer[0];
	while( *cp != '\n') {if( *cp == '\t' ) *cp = ' '; cp++;}
	cp = &buffer[0];
// this parsing is fragile.
	while( *cp != ' ') cp++;
	while( *cp == ' ') cp++;
	while( *cp != ' ') cp++;
	while( *cp == ' ') cp++;
	while( *cp != ' ') cp++;
	while( *cp == ' ') cp++; 

// assumes single letter categories. Beware
	while(*cp != ' ' ){ 
	if( *cp == '\n') break;
	if( *cp == 13) break;
	if( *cp == '\0') break;
	classid.push_back( *cp); cp++;
	while( *cp  == ' ') cp++;
	if( *cp == '\n') break;
	if( *cp == 13) break;
	if( *cp == '\0') break;
	}


	if( fgets(buffer, sizeof(buffer), ip) != buffer) return false;
	if( buffer[0] != '@') return false; // it's an error

	nfeature *=3;
	
	while( fgets(buffer, sizeof(buffer), ip) == buffer) 
	{
	// strip ',' and '\n'
	for( int i=0; i< sizeof(buffer); i++)
		if( buffer[i] == ',') buffer[i] = ' ';
		else if( buffer[i] == '\n'){ buffer[i] = ' '; break;}

	cp = &buffer[0];
	float attr[nfeature];
	int myid;
	for( int i=0; i< nfeature; i+=3)
	{
	while(*cp == ' ') cp++;
	sscanf(cp,"%f",&attr[i]);	
	while(*cp != ' ') cp++;
	}// i
//	printf("%s\n", cp);
	while(*cp == ' ') cp++;
	myid = -1;
	for( int i=0; i< classid.size(); i++)
		if( *cp == classid[i]){myid = i; break;}
	if( myid < 0) return false; // an error - unspecified class


	the_data.push_back( new kdd_datum( myid, nfeature, attr));

//	printf("%d ",myid);
//	for( int i=0; i< nfeature; i++)
//	printf("%f ", attr[i]);
//	printf("\n");

	}// while fgets
// normalize;
#define NORMALIZE_BY_CASE
#ifdef NORMALIZE_BY_CASE
	float min[nfeature],max[nfeature];
	for( int i=0; i< nfeature; i+=3)
		min[i] = max[i] = the_data[0]->data[i];
	for( int i=1; i< the_data.size(); i++)
	for( int j=0; j< nfeature; j+=3)
	{
		if( min[j] > the_data[i]->data[j]) min[j] = the_data[i]->data[j];
		if( max[j] < the_data[i]->data[j]) max[j] = the_data[i]->data[j];
	}
	for( int i=0; i< the_data.size(); i++)
	for( int j=0; j< nfeature; j+=3)
	{
		float x;
		x = the_data[i]->data[j];
		if( max[j] != min[j])
		the_data[i]->data[j] = (x-min[j])/(max[j]-min[j]);
		else
		the_data[i]->data[j] = 0.;
//		printf("%f %f\n", x,the_data[i]->data[j]);
	}	
#else
	float min,max;
	min = max = the_data[0]->data[0]; // pick a feasible value
	for( int i=1; i< the_data.size(); i++)
	for( int j=0; j< nfeature; j+=3)
	{
		if( min > the_data[i]->data[j]) min = the_data[i]->data[j];
		if( max < the_data[i]->data[j]) max = the_data[i]->data[j];
	}
	
	for( int i=0; i< the_data.size(); i++)
	for( int j=0; j< nfeature; j+=3)
	{
		float x;
		x = the_data[i]->data[j];
		the_data[i]->data[j] = (x-min)/(max-min);
	}
#endif

	for( int i=0; i< the_data.size(); i++)
	for( int j=0; j< nfeature; j+=3)
	{
		float x;
		x = the_data[i]->data[j];
		if( x >=0. && x < 1./8){ the_data[i]->data[j] = -1.F; the_data[i]->data[j+1] = -1.F; the_data[i]->data[j+2] = -1.F; continue;}
		if( x >=1./8 && x < 2./8){ the_data[i]->data[j] = -1.F; the_data[i]->data[j+1] = -1.F; the_data[i]->data[j+2] =  1.F; continue;}
		if( x >=2./8 && x < 3./8){ the_data[i]->data[j] = -1.F; the_data[i]->data[j+1] =  1.F; the_data[i]->data[j+2] =  1.F; continue;}
		if( x >=3./8 && x < 4./8){ the_data[i]->data[j] = -1.F; the_data[i]->data[j+1] =  1.F; the_data[i]->data[j+2] =  1.F; continue;}
		if( x >=4./8 && x < 5./8){ the_data[i]->data[j] =  1.F; the_data[i]->data[j+1] = -1.F; the_data[i]->data[j+2] = -1.F; continue;}
		if( x >=5./8 && x < 6./8){ the_data[i]->data[j] =  1.F; the_data[i]->data[j+1] = -1.F; the_data[i]->data[j+2] =  1.F; continue;}
		if( x >=6./8 && x < 7./8){ the_data[i]->data[j] =  1.F; the_data[i]->data[j+1] =  1.F; the_data[i]->data[j+2] = -1.F; continue;}
		if( x >=7./8 ){ the_data[i]->data[j] =  1.F; the_data[i]->data[j+1] =  1.F; the_data[i]->data[j+2] =  1.F; continue;}
	}



	return true;
}


