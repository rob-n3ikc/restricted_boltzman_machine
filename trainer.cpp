// multiclass style

#include "fmachina.h"
#include "mnist.h"

int main()
{
//RBM *a = new RBM;
RBM *a[10];
for( int i=0; i< 10; i++)
	a[i] = new RBM;

const int window_size = WIDTH*WIDTH; // defined in mnist.h
const int hidden_size = 10;
//const int hidden_size = 100;
//const int hidden_size = 1000;
//const int hidden_size = 2000;
//const int hidden_size = 3000;

FILE *ip; 
//ip = fopen("pdbaanr","r");
//ip = fopen("lysozyme","r");
//ip = fopen("helix_filt.dat","r");
//ip = fopen("mnist_test.csv","r");
ip = fopen("mnist_train.csv","r");

mnist *d = new mnist(ip);

static float encoded[window_size];

for( int i=0; i< 10; i++)
	a[i]->build(hidden_size,window_size);  

for( int i=1; i< 29; i++)
{

while(d->get_next())
{
   // if( d->threshold(100 ))
	int who;
	who = d->id;
    if( d->threshold(128 ))
	{
	for( int ie=0; ie< WIDTH*WIDTH; ie++)
		encoded[ie] = (float)(d->raw[ie]-128)/128;
    // a->train(who++,(float)0.01,window_size*code_size,encoded);
    // a->accuracy_train((float)0.01,window_size*code_size,encoded);
	if( i < 2)
  	 a[who]->train_o_matic((float)0.01,window_size,encoded);
	else
  	 a[who]->best_train_o_matic((float)0.01,window_size,encoded);
//     a[who]->train_o_matic((float)0.1,window_size,encoded);
	}

}
d->rewind_me();
	for( int who=0; who < 10; who++)
	{
		FILE *output;
		char line[80];
		sprintf(line,"window_%d_round_%d.rbm", who,i);
		output = fopen(line,"w");
		a[who]->dump(output);
		fclose(output);
	}
}//i
}
