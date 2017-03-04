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
float fsign;
std::vector<void*> links;
};



class connect{

public:
connect( node*, node*); 
node *h,*v;
float weight;
float e();
float ef();
bool dump(FILE *);
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


class RBM {
std::vector<node*> hidden;
std::vector<node*> visible;
std::vector<connect*> links;
int tokenizer( char** ,char*, int);
bool line_parser(char*, int, FILE*);
bool build(); //this can build in the absense of connections.
bool build(int); //this can build in the absense of connections. convolutional
public:
bool build(int, int); //this can build in the absense of connections.
bool build(int, int, int); //this can build in the absense of connections. convolutional
bool initialize_links(); //
RBM();
~RBM();
bool input(FILE*);
bool dump(FILE*);

bool train(float, int, int*);
bool train(float, int, float*);
bool train_o_matic(float, int, int*);
bool train_o_matic(float, int, float*);
bool best_train_o_matic(float, int, float*);
bool anti_train_o_matic(float, float, int, float*);
bool CD_train(float, int, int*);
bool accuracy_train(float, int, int*);
bool train(int, float, int, int*);
float energy( int, int*);
float energy( int, float*);
float best_expert_energy( int, int*);
float best_expert_energy( int, float*);
float best_raw_expert_energy( int, int*);
float best_raw_expert_energy( int, float*);
float fillin( int, int*); // zeros are assigned to +- one return the best energy found. 
float fillin( int, float*); // zeros are assigned to +- one return the best energy found. 
bool reconstruct_best( int, float *, float *); // find the best energy and use the hidden from that to select a new visible
bool reconstruct( int, float *, float *); // use the best hidden to select a new visible; use all the hiddens

};



