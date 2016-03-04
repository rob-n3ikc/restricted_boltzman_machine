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
// codec is binary encoding of aa
        codec[0][0] = -1;
        codec[1][0] = -1;
        codec[2][0] = -1;
        codec[3][0] = -1;
        codec[4][0] = -1;
        codec[0][1] =  1;
        codec[1][1] = -1;
        codec[2][1] = -1;
        codec[3][1] = -1;
        codec[4][1] = -1;
        codec[0][2] = -1;
        codec[1][2] =  1;
        codec[2][2] = -1;
        codec[3][2] = -1;
        codec[4][2] = -1;
        codec[0][3] =  1;
        codec[1][3] =  1;
        codec[2][3] = -1;
        codec[3][3] = -1;
        codec[4][3] = -1;
        codec[0][4] = -1;
        codec[1][4] = -1;
        codec[2][4] =  1;
        codec[3][4] = -1;
        codec[4][4] = -1;
        codec[0][5] =  1;
        codec[1][5] = -1;
        codec[2][5] =  1;
        codec[3][5] = -1;
        codec[4][5] = -1;
        codec[0][6] = -1;
        codec[1][6] =  1;
        codec[2][6] =  1;
        codec[3][6] = -1;
        codec[4][6] = -1;
        codec[0][7] =  1;
        codec[1][7] =  1;
        codec[2][7] =  1;
        codec[3][7] = -1;
        codec[4][7] = -1;
        codec[0][8] = -1;
        codec[1][8] = -1;
        codec[2][8] = -1;
        codec[3][8] =  1;
        codec[4][8] = -1;
        codec[0][9] =  1;
        codec[1][9] = -1;
        codec[2][9] = -1;
        codec[3][9] =  1;
        codec[4][9] = -1;
        codec[0][10] = -1;
        codec[1][10] =  1;
        codec[2][10] = -1;
        codec[3][10] =  1;
        codec[4][10] = -1;
        codec[0][11] =  1;
        codec[1][11] =  1;
        codec[2][11] = -1;
        codec[3][11] =  1;
        codec[4][11] = -1;
        codec[0][12] = -1;
        codec[1][12] = -1;
        codec[2][12] =  1;
        codec[3][12] =  1;
        codec[4][12] = -1;
        codec[0][13] =  1;
        codec[1][13] = -1;
        codec[2][13] =  1;
        codec[3][13] =  1;
        codec[4][13] = -1;
        codec[0][14] = -1;
        codec[1][14] =  1;
        codec[2][14] =  1;
        codec[3][14] =  1;
        codec[4][14] = -1;
        codec[0][15] =  1;
        codec[1][15] =  1;
        codec[2][15] =  1;
        codec[3][15] =  1;
        codec[4][15] = -1;
        codec[0][16] = -1;
        codec[1][16] = -1;
        codec[2][16] = -1;
        codec[3][16] = -1;
        codec[4][16] =  1;
        codec[0][17] =  1;
        codec[1][17] = -1;
        codec[2][17] = -1;
        codec[3][17] = -1;
        codec[4][17] =  1;
        codec[0][18] = -1;
        codec[1][18] =  1;
        codec[2][18] = -1;
        codec[3][18] = -1;
        codec[4][18] =  1;
        codec[0][19] =  1;
        codec[1][19] =  1;
        codec[2][19] = -1;
        codec[3][19] = -1;
        codec[4][19] =  1;
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

	char line[100];
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
	if( inseq < window) return false;
	if( which_window+window >= inseq) return false;
	for( int i=0; i< window; i++)
	{
		char c;
		c = seq[which_window+i];
		for( int j=0; j< 20; j++)
			if( single[j] == c)
			{
			   for( int k=0; k< 5; k++)
			    encoded[5*i+k] = codec[k][j];
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
			{
			bool all_match;
			all_match = true;
			for( int k=0;k<5; k++)
				if( encoded[i*5+k] != codec[k][j])
					{all_match=false; break;}
			

				if( all_match){
				decoded[i] = single[j];
				break;
				}
			}
	}//i  

	return true;
}
