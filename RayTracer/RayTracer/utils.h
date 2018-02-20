#ifndef UTILSH
#define UTILSH

static unsigned int g_seed;


//Used to seed the generator.

inline void fast_srand(int seed)

{

	g_seed = seed;

}


//fastrand routine returns one integer, similar output value range as C lib.

inline float fastrand()

{

	g_seed = (214013 * g_seed + 2531011);

	float output = ((g_seed >> 16) & 0x7FFF) / 10024.02324;

	return output - (int) output;

}

#endif

