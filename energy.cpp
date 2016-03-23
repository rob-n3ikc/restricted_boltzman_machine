
#include "machina.h"
#include "fasta.h"

const int code_size = 11;
const int window_size = 20;

int main()
{
RBM *a = new RBM;

FILE *ip; 
ip = fopen("one.10.rbm","r");
a->input(ip);
fclose(ip);
ip = fopen("pdbaanr","r");

fasta *d = new fasta(ip);

// static issue WTF man
static int encoded[window_size*code_size];
static int test[window_size*code_size];



static char wind[window_size+1];
while(d->get_next())
  while( d->window(window_size,wind))
  {
  d->encode(window_size,window_size*code_size,encoded);
	wind[window_size] = '\0';


	printf("%s %f\n",wind, a->best_expert_energy(window_size*code_size,encoded));
	
  }
}
