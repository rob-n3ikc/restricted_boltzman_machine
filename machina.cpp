// the machine definition file.
#include "machina.h"
#include <cstring>

using namespace std;

RBM::RBM()
{
// initialize the vectors.
//hidden.reserve(100);
//visible.reserve(100);
//links.reserve(100);
}
// vector_name.push_back(data)
// fill
bool RBM::line_parser(char* where, int max , FILE *what)
{
	for( int i=0; i< max; i++)
		where[i] = '\0';
	for( int i=0; i< max; i++)
	{
		where[i] = fgetc( what);
		if( where[i] == ';'){
			return true;
		}		
	}
	return false;
}
// negative return on an error. 
// either too many tokens or none
int RBM::tokenizer(  char **starts, char *data, int max)
{
	char *cp;
	int inme;
	inme = 0;
	cp = data;
	while( *cp != ';' || *cp != '\0')
	{
		while(*cp < '-' && *cp != ';'){ *cp = '\0'; cp++;}
		if( *cp == ';'){ *cp = '\0'; return inme;}
		starts[inme++] = cp;
		while( *cp >= '-' && *cp != ';') cp++;
		if( inme == max) return -inme;
	}
	return -1;
}

#include <cstring>
bool RBM::input( FILE *what)
{
	char line[4000];
	char *tokens[100];
	int max_token = 100;
	while( line_parser(line, sizeof(line)/sizeof(char), what) )
	{
	int inme;
	inme = tokenizer(tokens, line, max_token);
// show it works.
//	printf("%d\n", inme);
//	if( inme > 0)
//	for( int i=0; i< inme; i++)
//		printf("%d %s ", i,tokens[i]);
//	printf("\n");
	if( strcmp("build", tokens[0]) >0){ return build(); }
	char *cp;
	cp = tokens[0];
	if( strcmp("hidden", tokens[0]) ==0) {
		if( inme <2) {fprintf(stderr,"hidden needs two arguments\n"); exit(0);}

		int c;
		sscanf(tokens[1],"%d",&c);
		if( c <1) {fprintf(stderr,"hidden needs more than 0 nodes \n"); exit(0);}
		for( int i=0; i< c; i++)
		{
			hidden.push_back(new hidden_node);
			hidden[i]->serial = i;
			hidden[i]->hidden_node = true;
		}
		continue;

	}// hidden
	if( strcmp("visible", tokens[0]) ==0) {
		if( inme <2) {fprintf(stderr,"visible needs two arguments\n"); exit(0);}

		int c;
		sscanf(tokens[1],"%d",&c);
		if( c <1) {fprintf(stderr,"visible needs more than 0 nodes \n"); exit(0);}
		for( int i=0; i< c; i++)
		{
			visible.push_back(new visible_node);
			visible[i]->serial = i;
			visible[i]->hidden_node = false;
		}
		continue;

	}// visible 
	if( strcmp("connect", tokens[0]) ==0) {
		if( inme <6) {fprintf(stderr,"visible needs six arguments\n"); exit(0);}
		int a,b;
		sscanf(tokens[2],"%d",&a);
		sscanf(tokens[4],"%d",&b);
		if( *tokens[1] == 'h' && *tokens[3] == 'v')
		{
		links.push_back( new connect(hidden[a],visible[b]));
 		hidden[a]->links.push_back(links[links.size()-1]);
		}

		else if( *tokens[1] == 'h' && *tokens[3] == 'h')
		links.push_back( new connect(hidden[a],hidden[b]));

		else if( *tokens[1] == 'v' && *tokens[3] == 'v')
		links.push_back( new connect(visible[a],visible[b]));

		float w;
		sscanf(tokens[5],"%f",&w);
		links[links.size()-1]->weight = w;
	
	}//connect
	}
	return true;
}

// only called on a blank RBM
bool RBM::build(int nhide, int nvis )
{
		for( int i=0; i< nhide; i++)
		{
			hidden.push_back(new hidden_node);
			hidden[i]->serial = i;
			hidden[i]->hidden_node = true;
		}
		for( int i=0; i< nvis; i++)
		{
			visible.push_back(new visible_node);
			visible[i]->serial = i;
			visible[i]->hidden_node = false;
		}
		build();
}
// convolutional version
bool RBM::build(int nhide, int nvis ,int spread)
{
		for( int i=0; i< nhide; i++)
		{
			hidden.push_back(new hidden_node);
			hidden[i]->serial = i;
			hidden[i]->hidden_node = true;
		}
		for( int i=0; i< nvis; i++)
		{
			visible.push_back(new visible_node);
			visible[i]->serial = i;
			visible[i]->hidden_node = false;
		}
		build(spread);
}
bool RBM::build()
{
	for( int i=0; i< hidden.size() ; i++)
	{
		for( int j=0; j< visible.size(); j++)
		{
		links.push_back( new connect(hidden[i],visible[j]));
		links[links.size()-1]->weight = 0.;
		hidden[i]->links.push_back( links[links.size()-1]);
		}

	}
//	for( int i=0; i< hidden.size()-1 ; i++)
//	{
//		for( int j=i+1; j< hidden.size(); j++)
//		{
//		links.push_back( new connect(hidden[i],hidden[j]));
//		links[links.size()-1]->weight = 0.;
//		hidden[i]->links.push_back( links[links.size()-1]);
//		}
//	}
/*
// This code  enjsures easd node is initialized differently
	for( int i=0; i< hidden.size() ; i++)
	{
		int j;
		j = i % hidden[i]->links.size();
		connect *link;
		link = (connect*)hidden[i]->links[j];
		link->weight = 1.;
	}
*/

}
// convolutional version
bool RBM::build(int spread)
{
	int delta;
	delta = hidden.size()/spread;
	if( delta < 1) delta = 1;
	for( int i=0; i< hidden.size() ; i++)
	{
		for( int j=0; j< visible.size(); j++)
//		for( int j=0; j< spread; j++)
		{
		int k;
		k = j+i*delta;
		if( k < 0) k += visible.size();
		if( k >= visible.size()) k -= visible.size();
		links.push_back( new connect(hidden[i],visible[k]));
		links[links.size()-1]->weight = 0.;
		hidden[i]->links.push_back( links[links.size()-1]);
		}

	}

}

bool RBM::dump( FILE *where)
{
	fprintf(where,"hidden %d; visible %d;\n", (int)hidden.size(),(int)visible.size());
	for(int i=0; i< links.size(); i++)
		links[i]->dump(where);
	return true;
}

float RBM::energy(int inme, int *data)
{
// there is a design tradeoff here
// if every hidden node needs all its connections then there is
// a nhidden*nvisible memory requirement
// instead we can just loop over the connections and continue as needed.
	if( visible.size() != inme) return 0.;

	float e,ep,em;
	e = 0.;
	for( int i=0; i< visible.size(); i++)
		visible[i]->sign = data[i]; // this can be replaced by a callback.
	for( int i=0; i< hidden.size(); i++)
	{
		ep = 0.;
		em = 0.;
	for( int j=0; j< hidden[i]->links.size(); j++)
	{
		connect* link;
		link = (connect*)hidden[i]->links[j];
//		if( links[j]->h->serial != i) continue;
//		if( !links[j]->h->hidden_node) continue;
// energy is summed over every link for +/- 
		hidden[i]->sign = 1;
		ep += link->e();
		hidden[i]->sign = -1;
		em += link->e();
// a fast way to do this with quadratic only  is
//               float et;
//               et = links[j]->e();
//               ep += et; em -= et;
// but that may not generalize well
//
	}//j
		if( em < ep){ e+= em; hidden[i]->sign = -1;}
		else {e += ep; hidden[i]->sign = 1;}
	}//i
	return e;
}

float RBM::best_expert_energy(int inme, int *data)
{
// there is a design tradeoff here
// if every hidden node needs all its connections then there is
// a nhidden*nvisible memory requirement
// instead we can just loop over the connections and continue as needed.
	if( visible.size() != inme) return 0.;

	float e,ep,em;
	float ebest ;
	ebest = 10.e10;
	e = 0.;
	for( int i=0; i< visible.size(); i++)
		visible[i]->sign = data[i]; // this can be replaced by a callback.
	for( int i=0; i< hidden.size(); i++)
	{
		ep = 0.;
		em = 0.;
	for( int j=0; j< hidden[i]->links.size(); j++)
	{
		connect* link;
		link = (connect*)hidden[i]->links[j];
//		if( links[j]->h->serial != i) continue;
//		if( !links[j]->h->hidden_node) continue;
// energy is summed over every link for +/- 
		hidden[i]->sign = 1;
		ep += link->e();
		hidden[i]->sign = -1;
		em += link->e();
// a fast way to do this with quadratic only  is
//               float et;
//               et = links[j]->e();
//               ep += et; em -= et;
// but that may not generalize well
//
	}//j
                if( em < ebest) ebest = em;
                if( ep < ebest) ebest = ep;
		if( em < ep){ e+= em; hidden[i]->sign = -1;}
		else {e += ep; hidden[i]->sign = 1;}
	}//i
	return ebest;
}


bool RBM::train(float beta, int inme, int *data)
{
	static int count = 0;
	if( visible.size() != inme) return false;
	float e;
	float edot;
	e = energy( inme, data);
// at this stage all the signs are at the minimum energy.
///	for( int i=0; i< hidden.size(); i++)
	int i;
	i = (count++)%hidden.size();
	{
	for( int j=0; j< hidden[i]->links.size(); j++)
	{
		connect *link;
		link = (connect*)hidden[i]->links[j];
		float ep,em, fp,fm, damp;
		int dot;
//		if( links[j]->h->serial != i) continue;
//		if( !links[j]->h->hidden_node) continue;
// Plus/Minus is arbitrary here
		ep = link->e()*beta *hidden[i]->sign;
		em = -ep;
//		if( ep > 20.) ep = 20.;
//		if( ep < -20.) ep = -20.;
		dot = link->h->sign * links[j]->v->sign;
		fp = exp( ep);
		fm = exp( em);  // safer
		damp =  (fp-fm)/(fp+fm);
		edot = (float)dot;
//		printf("%f %f %d %f\n", damp, ep,dot, edot + damp);
//		damp *= dot;
		damp *= dot*hidden[i]->sign;
		link->weight -= edot+damp;
//		link->weight -= edot;
	}//j
	}//i
	return true;
}

bool RBM::train(int i,float beta, int inme, int *data)
{
//	static int count = 0;
	if( visible.size() != inme) return false;
	float e;
	float edot;
	e = energy( inme, data);
// at this stage all the signs are at the minimum energy.
//	for( int i=0; i< hidden.size(); i++)
//	int i;
//	i = (count++)%hidden.size();
	i = i % hidden.size();
	{
	for( int j=0; j< hidden[i]->links.size(); j++)
	{
		connect *link;
		link = (connect*)hidden[i]->links[j];
		float ep,em, fp,fm, damp;
		int dot;
//		if( links[j]->h->serial != i) continue;
//		if( !links[j]->h->hidden_node) continue;
// Plus/Minus is arbitrary here
		ep = link->e()*beta *hidden[i]->sign;
		em = -ep;
//		if( ep > 20.) ep = 20.;
//		if( ep < -20.) ep = -20.;
		dot = link->h->sign * links[j]->v->sign;
		fp = exp( ep);
		fm = exp( em);  // safer
		damp =  (fp-fm)/(fp+fm);
		edot = (float)dot;
//		printf("%f %f %d %f\n", damp, ep,dot, edot + damp);
//		damp *= dot;
		damp *= dot*hidden[i]->sign;
		link->weight -= edot+damp;
//		link->weight -= edot;
	}//j
	}//i
	return true;
}


// +- 1 if known, 0 otherwise
float RBM::fillin( int inme, int *what)
{
// slow niave algorithm for each zero bit find the energy
	float ep,em;
	for( int i =0; i< inme; i++)
	{
		if( what[i] != 0) continue;
		what[i] = 1;
		ep = energy(inme,what);
		what[i] = -1;
		em = energy(inme,what);
		if( ep < em) what[i] = 1;

	}
		return  energy(inme,what);
}

node::node(){;;}
//hidden_node::hidden_node(){;;}
//visible_node::visible_node(){;;}

connect::connect(node *a, node *b)
{
 h = a; v = b; weight = 0.;
}
float connect::e()
{
	return weight *h->sign*v->sign;
}
bool connect::dump(FILE* where)
{
// h and v are virtual here
	if( h->hidden_node && !v->hidden_node)
	fprintf(where,"connect h %d v %d %f;\n", h->serial, v->serial, weight);
	if( h->hidden_node &&  v->hidden_node)
	fprintf(where,"connect h %d h %d %f;\n", h->serial, v->serial, weight);
	if( !h->hidden_node && !v->hidden_node)
	fprintf(where,"connect v %d v %d %f;\n", h->serial, v->serial, weight);
	if( !h->hidden_node &&  v->hidden_node)
	fprintf(where,"connect v %d h %d %f;\n", h->serial, v->serial, weight);
}
