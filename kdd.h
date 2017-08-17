// definition of the data block
//
// MNIST reworked for KDD format
//
//
// there is a header that gives 
//
//   nfield == nfeature + 1  (always f1,..., fn, class)
// @attribute (nfield times)  describing the attributes all ours are numeric
// @attribute  class labels.
// @data
// then the data in csv.
//
// 
// also, since these are small files, I'm going to save the data in memory.
#include <cstdio>
#include <vector>
// everything is public so I can find min,max and renormalize myself.
class kdd_datum{
public:
int id,inme;
float *data;
kdd_datum(int, int, float*); // category size, data
~kdd_datum(); // need to delete data ourselves.
};




class kdd{
public: // I'm using this to organize functions, nothing oddball
int iterator;
int nfeature;
std::vector< kdd_datum *> the_data;
std::vector<char> classid;

bool get_next(int&, int, float*); // get the next
bool rewind_me();
bool load(FILE*);
};
