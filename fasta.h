/* this supports reading, encoding and decoding a fasta file 
*
* for input into an RBM or similar thing
*/
#include <cstdio>
#include <cstdlib>

class fasta{
FILE *the_file;
int which_window;
public:
fasta(FILE *);
~fasta();
bool rewind_me();
bool get_next();
bool window(int, char*);
bool encode(int, int, int*);  // window, size of encoding, data
bool decode(int, int, int*, char*);  // window, size of encoding, data
char seq[4000];
int inseq;


};
