
#include "machina.h"

int main()
{
RBM *a = new RBM;

//a->input(stdin);
// for xor
a->build(1,4);  
int x[4],y[4],s[4],sz[4];
for( int i=0; i< 4; i++)
{
	x[i] = 1;
	y[i] = -1;
	s[i] = 1;
	sz[i] = -1;
}
for( int i=0; i<4; i+=3)
{
	x[i] = -1;
	y[i] =  1;
}
for( int i=0; i< 100; i++)
	{
a->train((float)1.001,4,x);
a->train((float)1.001,4,y);
printf( "xor %f %f same %f %f\n",a->energy(4,x),a->energy(4,y),a->energy(4,s),a->energy(4,sz));

a->dump(stdout);
	}

}
