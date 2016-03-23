#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int c, char **a)
{
float mean, m2, n,x;
char buffer[100];
char seq[100];
FILE *ip;

if( c < 2){ fprintf(stderr,"must give me the file name\n"); exit(-1);}
ip = fopen(a[1],"r");

mean = m2 = n = 0.0;
while( fgets(buffer,sizeof(buffer), ip) > (char *)0)
{
	sscanf( buffer,"%s %f",seq, &x);
	n += 1.;
	mean += x;
	m2 += x*x;
}
rewind(ip);
mean /=n;
m2 /= n;
m2 = sqrt(m2-mean*mean);
while( fgets(buffer,sizeof(buffer), ip) > (char *)0)
{
	float z;
	sscanf( buffer,"%s %f",seq, &x);
	z = (x-mean)/m2;	
	printf("%s %f %f\n",seq,x,z);

}

}
