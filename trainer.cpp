
#include "machina.h"
#include "fasta.h"

int main()
{
RBM *a = new RBM;

const int window_size = 20;
const int code_size = 11;
//const int hidden_size = 5;
const int hidden_size = 200;
//const int spread_size = 20;
const int spread_size = 20*code_size;

FILE *ip; 
//ip = fopen("pdbaanr","r");
ip = fopen("lysozyme","r");

fasta *d = new fasta(ip);

// static issue WTF man
static int encoded[window_size*code_size];
static char wind[window_size+1];
a->build(hidden_size,window_size*code_size);  
//a->build(hidden_size,window_size*code_size);
for( int i=1; i< 30; i++)
{

while(d->get_next())
{
  int who;
  who = 0;
  while( d->encode(window_size,window_size*code_size,encoded))
  {
     d->window(window_size, wind);
	printf("%d %s\n", who, wind);
    // a->train(who++,(float)0.01,window_size*code_size,encoded);
     a->train(who++,(float)0.01,window_size*code_size,encoded);
  }

}
d->rewind_me();
FILE *output;
char line[80];
sprintf(line,"window_%d_round_%d.rbm", window_size,i);
output = fopen(line,"w");
a->dump(output);
fclose(output);
}//i
}
