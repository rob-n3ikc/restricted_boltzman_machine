
#include "machina.h"
#include "fasta.h"

int main()
{
const int code_width = 5;
const int window_width = 40;
RBM *a = new RBM;

FILE *ip; 
ip = fopen("pdbaanr","r");

fasta *d = new fasta(ip);

// static issue WTF man
static int encoded[window_width*code_width];
a->build(code_width*window_width,window_width*code_width);  
for( int i=1; i< 10; i++)
{

while(d->get_next())
  while( d->encode(window_width,window_width*code_width,encoded))
  {
//	for(int k=0; k< window_width*code_width; k++)
//		printf("%d ", encoded[k]);
//	printf("\n");
     a->train((float)0.01,window_width*code_width,encoded);
  }
d->rewind_me();
FILE *output;
char line[80];
sprintf(line,"window_%d_round_%d.rbm", window_width,i);
output = fopen(line,"w");
a->dump(output);
fclose(output);
}//i
}
