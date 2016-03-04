
#include "machina.h"
#include "fasta.h"

int main()
{
RBM *a = new RBM;
const int code_width = 5;
const int window_width = 40;

FILE *ip; 
ip = fopen("one.10.rbm","r");
a->input(ip);
fclose(ip);
ip = fopen("pdbaanr","r");

fasta *d = new fasta(ip);

// static issue WTF man
static int encoded[window_width*code_width];
static int test[window_width*code_width];



while(d->get_next())
  while( d->encode(window_width,window_width*code_width,encoded))
  {
	int same;

	for( int ia=0;ia<window_width; ia++)
	{
	same = 0;
	for( int i=0; i< window_width*code_width; i++)
		test[i] = encoded[i];
	for( int i=ia*code_width; i< ia*code_width+code_width; i++)
		test[i] = 0;
//	for( int i=ia*code_width; i< ia*code_width+code_width; i++)
//		printf("%d %d\n", encoded[i],test[i]);

	a->fillin(window_width*code_width, test);
//	for( int i=ia*code_width; i< ia*code_width+code_width; i++)
//		printf("%d %d\n", encoded[i],test[i]);
	for( int i=ia*code_width; i< ia*code_width+code_width; i++)
		if( test[i] == encoded[i]) same ++;
	printf("%d ", same);
   
	
	}
	printf("\n");
//     a->train((float)1.001,window_width*code_width,encoded);
  }
}
