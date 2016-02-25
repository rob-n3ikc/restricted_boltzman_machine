/* this is the general .h file for the machine
//
// it includes the definitions of nodes, link geometry, and overall
// parser/framework
//
//
// use the autovector construct when possible
//
*/

// we'll use FILE*
// Lump it.

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <vector>

class node{
public: int state;
node();
int serial;
bool hidden_node;
int sign;
};

// hidden and public are differentiated so that a visible node can
// include a callback to map aribrary data to a signed value.
// zB preclassification with an SVM.
// Since hidden are only visible internally they can obey the +- 1 rule.
class hidden_node:public node{

public: 

};

class visible_node:public node{
public:


};



class connect{

public:
connect( node*, node*); 
node *h,*v;
float weight;
float e();
bool dump(FILE *);
};

class RBM {
std::vector<node*> hidden;
std::vector<node*> visible;
std::vector<connect*> links;
int tokenizer( char** ,char*, int);
bool line_parser(char*, int, FILE*);
bool build(); //this can build in the absense of connections.
public:
bool build(int, int); //this can build in the absense of connections.
RBM();
~RBM();
bool input(FILE*);
bool dump(FILE*);

bool train(float, int, int*);
float energy( int, int*);
float fillin( int, int*); // zeros are assigned to +- one return the best energy found. 

};



