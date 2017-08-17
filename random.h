//#ifndef RANDOM_RNG_INCLUDED
//#define RANDOM_RNG_INCLUDED
class random {

	float buff[55];
	int ip,jp,kp;
public: random();
		random(int);
		~random();
		float value();
		float gaussian();
		float cauchy();
		void sphere(float *, float *, float *);
		void nbox(int , float*);
private: void initialize_buffer(int);
};
//#endif
