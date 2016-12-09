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
 		visible[b]->links.push_back(links[links.size()-1]);
		}

		if( *tokens[1] == 'v' && *tokens[3] == 'h')
		{
		links.push_back( new connect(hidden[b],visible[a]));
 		hidden[b]->links.push_back(links[links.size()-1]);
 		visible[a]->links.push_back(links[links.size()-1]);
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
//		links[links.size()-1]->weight = (float)rand()/RAND_MAX;
		hidden[i]->links.push_back( links[links.size()-1]);
		visible[j]->links.push_back( links[links.size()-1]);
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
	float enorm;
	float ebest ;
	ebest = 10.e10;
	e = 0.;
	for( int i=0; i< visible.size(); i++)
		visible[i]->sign = data[i]; // this can be replaced by a callback.
	for( int i=0; i< hidden.size(); i++)
	{
		ep = 0.;
		em = 0.;
		enorm = 0.;
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
		enorm += fabs(link->weight);
// a fast way to do this with quadratic only  is
//               float et;
//               et = links[j]->e();
//               ep += et; em -= et;
// but that may not generalize well
//
	}//j
		if( enorm > 0.)
		{
			em /= enorm; ep /= enorm;
		}
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

bool RBM::train_o_matic(float beta, int inme, int *data)
{
	if( visible.size() != inme) return false;
	float e;
	float edot;
	e = energy( inme, data);
// at this stage all the signs are at the minimum energy.
	float eworst,ebest;
	int iworst, ibest;
	ebest = 10.e10; eworst = -10.e10;
	iworst = -1; ibest = -1;
	for( int i=0; i< hidden.size(); i++)
	{
// find the best and worst energy links
	float my_energy,my_norm;
	my_energy = my_norm = 0.;
	for( int j=0; j< hidden[i]->links.size(); j++)
	{
		connect *link;
		link = (connect*)hidden[i]->links[j];
		my_energy +=  link->e() ;
		my_norm += fabs(link->weight);
	}//j
	if( my_norm > 0.) my_energy/=my_norm;
	if( my_energy < ebest) {ebest = my_energy; ibest = i;}
	if( my_energy > eworst) {eworst = my_energy; iworst = i;}
	}//i
	printf("%f %d %f %d\n",eworst, iworst, ebest,ibest);
	for( int i=0; i< hidden.size(); i++)
	{
	if( !(i == ibest or i == iworst)) continue;
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

// this is the "standard" algorithm
// this is a wrong implementation.
// the prediction of v is not right
// see accuracy_train
bool RBM::CD_train(float beta, int inme, int *data)
{
	static int count = 0;
	if( visible.size() != inme) return false;
	float e;
	float edot;
	e = energy( inme, data);
// at this stage all the signs are at the minimum energy.
// that's the same for standard and non-standard algorithms.
	for( int i=0; i< hidden.size(); i++)
	for( int j=0; j< hidden[i]->links.size(); j++)
	{
		connect *link;
		link = (connect*)hidden[i]->links[j];
		node *v;
		v = link->v; // easier typing.
		float ep,em, fp,fm, damp;
		float v_predicted;
// predict what the visible should be for my hidden value.
// standard quadratic model.
		ep = link->weight*hidden[i]->sign;
		em = -ep;
		if( ep < em)
		{
			edot = -v->sign*hidden[i]->sign +hidden[i]->sign;

		} else{
			edot =  v->sign*hidden[i]->sign -hidden[i]->sign;

		}
		
		link->weight -= 0.1*edot;
	}//j
	return true;
}

bool RBM::accuracy_train(float beta, int inme, int *data)
{
// original version was wrong.
	static int count = 0;
	if( visible.size() != inme) return false;
	for( int l=0; l< 2; l++)
	{
	float e;
	float edot;
	float accuracy;
	accuracy = 0.;
	e = energy( inme, data);
	printf("%f \n", e);
// at this stage all the signs are at the minimum energy.
// that's the same for standard and non-standard algorithms.
	for( int k=0; k< visible.size(); k++)
	{
		float e_predicted_plus;
		float e_predicted_minus;
		node *v;
		v = visible[k]; // easier typing.
		e_predicted_plus = 0.;
		e_predicted_minus = 0.;
//	for( int i=0; i< hidden.size(); i++)
//	for( int j=0; j< hidden[i]->links.size(); j++)
	for( int j=0; j< visible[k]->links.size(); j++)
	{
		connect *link;
	//	link = (connect*)hidden[i]->links[j];
		link = (connect*)visible[k]->links[j];
		float ep,em, fp,fm, damp;
		node *vv,*hh;
		vv = link->v; // easier typing.
		hh = link->h; // easier typing.
// warning the next line could be fragile 
//		if( vv != v) continue; // address alias for id
// predict what the visible should be for my hidden value.
// standard quadratic model.
	//	ep = link->weight*hidden[i]->sign;
		ep = link->weight*hh->sign;
		e_predicted_plus += ep;
		e_predicted_minus -= ep;
		
	}//j
// so now did I get it right?
		bool correct;
		correct = false;
		if( e_predicted_plus > e_predicted_minus and v->sign < 0.) correct = true;
		if( e_predicted_plus < e_predicted_minus and v->sign > 0.) correct = true;
		if( correct) accuracy += 1.;
		float fp,fm,damp;
		fp = exp( e_predicted_plus*beta);
		fm = exp( e_predicted_minus*beta);  // safer
		damp =  (fp-fm)/(fp+fm);
// now update the weights. 
// only for h-> v links
//	for( int i=0; i< hidden.size(); i++)
//	for( int j=0; j< hidden[i]->links.size(); j++)
	for( int j=0; j< visible[k]->links.size(); j++)
	{
		connect *link;
	//	link = (connect*)hidden[i]->links[j];
		link = (connect*)visible[k]->links[j];
		node *vv;
		vv = link->v; // easier typing.
// warning the next line could be fragile 
	//	if( vv != v) continue; // address alias for id
		float edot;
	//	edot = hidden[i]->sign*(v->sign+damp);
	//	edot = hidden[i]->sign*(v->sign)*0.01;
	//	edot = hidden[i]->sign*(v->sign)*0.1;
		edot = link->h->sign*(v->sign);
// the random is to force the different layers to be different
		link->weight -= edot*0.01*rand()/RAND_MAX;

/*
		if( correct)
		{
			link->weight -= edot ;
		}else{
			link->weight += edot ;
		}	
*/
	

	}// j (second time)

// renormalize
#ifdef renormalize
	float max;
	max = 0.;
	for( int j=0; j< visible[k]->links.size(); j++)
	{
		connect *link;
		link = (connect*)visible[k]->links[j];
		if( max <  fabs(link->weight))
			max = fabs(link->weight);
	}
	if( max > 1.e-7)
	for( int j=0; j< visible[k]->links.size(); j++)
	{
		connect *link;
		link = (connect*)visible[k]->links[j];
		link->weight /= max;
	}

#endif
	}//k
	printf("accuracy %f\n", accuracy/visible.size());
	}//l
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
