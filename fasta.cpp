/* this supports reading, encoding and decoding a fasta file 
*
* for input into an RBM or similar thing
*/
#include <cstdio>
#include <cstdlib>
#include "fasta.h"

/*
* initial class def
class fasta{
FILE *the_file;
char *single;
int which_window;
public:
fasta(FILE *);
~fasta();
bool rewind();
bool get_next();
bool encode(int, int, int*);  // window, size of encoding, data
bool decode(int, int, int*, char*);  // window, size of encoding, data
char seq[4000];
int inseq;
};
*/
//
// which_window logic is in get_next()
// usage would be x = new fasta(FILE * ...);
// for(...)
//  while( get_next())
//    while( encode(...) ) train(...)
//

fasta::fasta(FILE *where)
{
	the_file = where;
}
fasta::~fasta(){ fclose( the_file);}

bool fasta::rewind_me(){ rewind(the_file); return true;}

bool isempty(char *cp)
{
	while(*cp != '\0')
	{
	if( *cp == '\n') return true;
        if( *cp != ' ') return false;
	cp++;		
	}	
	return true;
}

bool fasta::get_next()
{

	char line[1000];
	inseq = 0;
	for( int i=0; i< sizeof(seq); i++)
		seq[i] = '\0';
	which_window = 0;

	while( fgets(line, sizeof(line), the_file)!= (char*)0)
	{

		if( line[0] == '>') continue;
		if( inseq > 0 && isempty(line)) return true;
		char *cp;
		cp = line;
		while(*cp != '\n' && *cp != '\0')
		{
		if( *cp != ' ' ) seq[inseq++] = *cp;
		cp++;
		}
	}
	if( inseq >0) return true;
	return false;
}

bool fasta::encode(int window, int codesize, int *encoded)
{
const char *single = "ANDRCGQEHILKMFPSTYWV";
const char *hydro = "0NDR00QEH00K000STY00";
const char *hydrP = "0000C0000IL0MFP00YWV";
const char *charge = "00DR000EH00K00000000";
const char *aroma = "000R0000H0000F000YW0";
const char *big = "000RC0QEHILKMF000YW0";
const char *posit = "000R0000H00K00000000";
const char *negat = "00D0000E000000000000";
const char *active= "00D0C000H000000S0000";
const char *cys = "0000C000000000000000";
const char *turn = "A0000G00000000P00000";
const char *recog = "0N0R00Q0000K0F0STY00";
	if( inseq < window) return false;
	if( which_window+window +1 >= inseq) return false;
	for( int i=0; i< codesize; i++)
		encoded[i] = -1;
	for( int i=0; i< window; i++)
	{
		char c;
		c = seq[which_window+i];
		for( int j=0; j< 20; j++)
			if( single[j] == c)
			{
#ifdef use_id_as_vector
			    encoded[25*i+j] = 1;
			    if( hydro[j] != '0') encoded[25*i+20] = 1;
			    if( hydrP[j] != '0') encoded[25*i+21] = 1;
			    if( charge[j] != '0') encoded[25*i+22] = 1;
			    if( aroma[j] != '0')  encoded[25*i+23] = 1;
			    if( big[j] != '0')    encoded[25*i+24] = 1;
#else
			    if( hydro[j] != '0')  encoded[11*i] = 1;
			    if( hydrP[j] != '0')  encoded[11*i+1] = 1;
			    if( charge[j] != '0') encoded[11*i+2] = 1;
			    if( aroma[j] != '0')  encoded[11*i+3] = 1;
			    if( big[j] != '0')    encoded[11*i+4] = 1;
			    if( posit[j] != '0')  encoded[11*i+5] = 1;
			    if( negat[j] != '0')  encoded[11*i+6] = 1;
			    if( active[j] != '0') encoded[11*i+7] = 1;
			    if( cys[j] != '0')    encoded[11*i+8] = 1;
			    if( turn[j] != '0')   encoded[11*i+9] = 1;
			    if( recog[j] != '0')  encoded[11*i+10] = 1;

#endif
		      	    break;
			}
	}
	which_window+= window;
//	which_window++;

	return true;
}
//bool decode(int, int, int*, char*);  // window, size of encoding, data
bool fasta::decode(int window, int codesize, int *encoded, char *decoded)
{
const char *single = "ANDRCGQEHILKMFPSTYWV";
	
	for(int i=0; i< window; i++)
	{
		decoded[i] = '*'; // no result
		for( int j=0; j< 20; j++)
			if( encoded[20*i+j] > 0)
			{
				decoded[i] = single[j];
				break;
			}
	}//i  

	return true;
}

bool fasta::window( int size, char *where)
{
	if( inseq < size) return false;
	if( which_window+size +1 >= inseq) return false;
	for( int i=0;i< size;i++)
		where[i] = seq[i+which_window];
	return true;
}
