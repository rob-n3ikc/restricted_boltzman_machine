#include "mnist.h"
// class definitions for the file

// nothing special in the constructors
// so use the defaults
mnist::mnist(FILE *x){ip = x;}
//mnist::~mnist(){;}

bool mnist::get_next()
{
	return load(ip);
}

bool mnist::get_next(int who)
{
	do{
	if(!load(ip)) return false;
	}while( id != who);
	return true;
}

bool mnist::rewind_me(){ rewind(ip);}

bool mnist::load( FILE *jp)
{
	char buffer[WIDTH*WIDTH*5+10];
	if( fgets(buffer, sizeof(buffer), jp) != buffer) return false;
	// strip ',' and '\n'
	for( int i=0; i< WIDTH*WIDTH*5+10; i++)
		if( buffer[i] == ',') buffer[i] = ' ';
		else if( buffer[i] == '\n'){ buffer[i] = ' '; break;}
	char *cp;
	cp = &buffer[0];
	while( *cp == ' ') cp++;
	sscanf(cp,"%d", &id);
	while( *cp != ' ') cp++;
	while( *cp == ' ') cp++;
	for( int i=0; i< WIDTH*WIDTH; i++)
	{
	sscanf(cp,"%d", &raw[i]);
	while( *cp != ' ') cp++;
	while( *cp == ' ') cp++;
	if( *cp == '\0' && i < WIDTH*WIDTH-1) return false; // an error
	}
	return true;
}


bool mnist::threshold( int where)
{
	for( int i=0; i< WIDTH*WIDTH; i++)
		if( raw[i] > where) threshed[i] = 1;
		else threshed[i] = -1;
	return true;
}


bool mnist::xpm_reconstruct( FILE *op)
{
char *hex="0123456789abcdef";
char q = '"';
fprintf(op,"#define XFACE_format 1 \n#define XFACE_width 28 \n#define XFACE_height 28 \n#define XFACE_ncolors 256 \n");
fprintf(op,"#define XFACE_chars_per_pixel 2 \n");

fprintf(op,"static char *XFACE_colors[] = {\n");
// get the scale parameters
	float scale, minus;
	scale = minus = reconstructed[0];
	for( int i=0; i< WIDTH*WIDTH; i++)
	{
		if( minus > reconstructed[i]) minus = reconstructed[i];
		if( scale <  reconstructed[i]) scale = reconstructed[i];
	}
	scale -= minus;
	if( scale == 0.) return false;
	scale = 255.F/scale;
	for( int i=0; i< 256; i++)
	{
		int b,s;
		s = i%16;
		b = (i-s)/16;
		int bb,ss;
		bb = 15-b;
		ss = 15-s;
		
		fprintf(op, "%c%c%c%c, %c#%c%c%c%c%c%c%c", q,hex[b],hex[s],q,q, hex[bb],hex[ss],hex[bb],hex[ss],hex[bb],hex[ss],q);
		if( i < 255) fprintf(op, ",\n");
		else fprintf(op,"\n};\n");
	}

fprintf( op,"static char *XFACE_pixels[] = {\n%c",q);

	for( int i=0; i< WIDTH*WIDTH; i++)
	{
		int b,s;
		int ra;
		ra = (reconstructed[i]-minus)*scale;
		if( ra > 255) ra = 255;
		if( ra < 0) ra = 0;
		s = ra%16;
		b = (ra-s)/16;
		fprintf(op,"%c%c",hex[b],hex[s]);
		if( i%WIDTH == WIDTH-1 ) 
			{
				fprintf(op,"%c",q);
				if( i < WIDTH*WIDTH -WIDTH) fprintf(op,",\n%c",q);
				else fprintf(op,"\n};\n");	
			}

	}
	return true;
}

bool mnist::xpm_raw( FILE *op)
{
char *hex="0123456789abcdef";
char q = '"';
fprintf(op,"#define XFACE_format 1 \n#define XFACE_width 28 \n#define XFACE_height 28 \n#define XFACE_ncolors 256 \n");
fprintf(op,"#define XFACE_chars_per_pixel 2 \n");

fprintf(op,"static char *XFACE_colors[] = {\n");
	for( int i=0; i< 256; i++)
	{
		int b,s;
		s = i%16;
		b = (i-s)/16;
		int bb,ss;
		bb = 15-b;
		ss = 15-s;
		
		fprintf(op, "%c%c%c%c, %c#%c%c%c%c%c%c%c", q,hex[b],hex[s],q,q, hex[bb],hex[ss],hex[bb],hex[ss],hex[bb],hex[ss],q);
		if( i < 255) fprintf(op, ",\n");
		else fprintf(op,"\n};\n");
	}

fprintf( op,"static char *XFACE_pixels[] = {\n%c",q);

	for( int i=0; i< WIDTH*WIDTH; i++)
	{
		int b,s;
		s = raw[i]%16;
		b = (raw[i]-s)/16;
		fprintf(op,"%c%c",hex[b],hex[s]);
		if( i%WIDTH == WIDTH-1 ) 
			{
				fprintf(op,"%c",q);
				if( i < WIDTH*WIDTH -WIDTH) fprintf(op,",\n%c",q);
				else fprintf(op,"\n};\n");	
			}

	}
	return true;
}

