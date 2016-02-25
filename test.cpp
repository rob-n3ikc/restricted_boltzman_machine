#include "machina.h"

int main()
{
RBM *a = new RBM;

//a->input(stdin);
// for xor
a->build(2,2);  
int x[2],y[2],s[2],sz[2];
x[0] = -1;
x[1] =  1;
y[0] =  1;
y[1] = -1;
s[0] =  1;
s[1] =  1;
sz[0] = -1;
sz[1] = -1;
for( int i=0; i< 100; i++)
	{
a->train((float)1.001,2,x);
a->train((float)1.001,2,y);
printf( "xor %f %f same %f %f\n",a->energy(2,x),a->energy(2,y),a->energy(2,s),a->energy(2,sz));

a->dump(stdout);
	}

s[1] = 0;
a->fillin(2,s);
printf("%d %d\n",s[0],s[1]);
}
