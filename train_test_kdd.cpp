

// multiclass style

#include "bmachina.h"
#include "kdd.h"

int main( int argc, char **argv)
{
if( argc < 4){
	fprintf(stderr,"Usage: train_test nhidden train_file test_file\n");
	exit(0);
}
int hidden_size;
sscanf(argv[1],"%d", &hidden_size);
kdd *train = new kdd();
kdd *test = new kdd();
FILE *ip;
char line[80];
sscanf(argv[2],"%s",line);
ip = fopen(line,"r");
train->load(ip);
fclose(ip);
sscanf(argv[3],"%s",line);
ip = fopen(line,"r");
test->load(ip);
fclose(ip);

int nfeature;
nfeature = train->nfeature;
int nclass;
nclass = train->classid.size();

//RBM *a = new RBM;
RBM *a[nclass];
for( int i=0; i< nclass; i++)
	a[i] = new RBM;

int window_size = nfeature;



float *encoded; encoded = new float[window_size];

for( int i=0; i< nclass; i++)
{
	a[i]->build(hidden_size,window_size);  
	a[i]->clear_bias();
}


#ifdef BIAS
int who;
train->rewind_me();
while(train->get_next(who, nfeature, encoded))
{
	a[who]->add_to_bias(window_size, encoded);
}
train->rewind_me();

for( int i=0; i< nclass; i++)
	a[i]->normalize_bias();

#endif



train->rewind_me();

for( int i=1; i< 51; i++)
{

int who;
while(train->get_next( who, nfeature, encoded))
{
	if( i < 2)
  	 a[who]->train_o_matic((float)0.01,window_size,encoded);
	else
  	 a[who]->best_train_o_matic((float)0.01,window_size,encoded);

}
train->rewind_me();
	for( int who=0; who < nclass; who++)
	{
		FILE *output;
		char line[80];
		sprintf(line,"window_%d_round_%d.rbm", who,i);
		output = fopen(line,"w");
		a[who]->dump(output);
		fclose(output);
	}
// testing
	float inme, correct;
	inme = 0.F;
	correct = 0.F;
	test->rewind_me();
while( test->get_next( who, nfeature, encoded))
{
	int imin; float emin;
	imin = 0;
	float re[nfeature];
//	emin = -a[0]->reconstruct_best(window_size,encoded,re);
	emin =a[0]->best_expert_energy(window_size,encoded);
	for( int j=1; j< nclass; j++)
	{
		float e;
		e =a[j]->best_expert_energy(window_size,encoded);
//		e = -a[j]->reconstruct_best(window_size,encoded,re);
		if( e < emin)
		{
			emin = e;
			imin = j;
		}
	}

//	printf("%d %d %f %f\n", imin, who, emin,  a[who]->best_expert_energy(window_size,encoded));
//	printf("%d %d %f %f\n", imin, who, emin,  -a[who]->reconstruct_best(window_size,encoded,re));
//	for( int i=0; i< nfeature; i++)
//		printf("%f %f\n",encoded[i], re[i]);
	if( imin == who){ correct += 1.0F;}
	inme += 1.F;
    
}// testing while
	printf("%d %f\n", i, correct/inme);
	fflush(stdout);
	test->rewind_me();
}//i
}
