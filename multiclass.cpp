
#include "fmachina.h"
#include "mnist.h"

const int window_size = WIDTH*WIDTH;

int main()
{
RBM *a[10];

FILE *ip; 
for( int i=0; i< 10; i++)
{
	char line[80];
	sprintf(line, "%d.rbm",i);
	ip = fopen(line,"r");
	a[i] = new RBM;
	a[i]->input(ip);
	fclose(ip);
}
ip = fopen("mnist_test.csv","r");

mnist *d = new mnist(ip);

// static issue WTF man
static float encoded[window_size];
static int test[window_size];


int itotal,icorrect;
itotal = icorrect = 0;

while(d->get_next())
  {
  d->threshold(100);
	for( int i=0; i< window_size; i++)
		encoded[i] = (float)(d->raw[i]-128)/128;
	float e[10];
	for( int i=0; i< 10; i++)
		e[i] = a[i]->best_expert_energy(window_size,encoded);
	int ibest;
	float ebest;
	ibest = 0;
	ebest = e[ibest];
	for( int i=1; i< 10; i++)
		if( ebest > e[i]){ ebest = e[i]; ibest = i;}
	printf("%d %d %f %f\n",d->id, ibest,ebest,e[d->id]);
	itotal += 1;
	if( ibest == d->id) icorrect += 1;

//	printf("%d %f\n",d->id, a->best_expert_energy(window_size,encoded));
	
  }
	printf("%d %d %f\n", itotal ,icorrect, (float)icorrect/(float)itotal);
}
