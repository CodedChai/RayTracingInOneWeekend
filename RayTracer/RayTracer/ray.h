#ifndef RAYH
#define RAYH
#include "vec.h"

class ray {
public:
	vec o;
	vec d;
	ray() { }
	ray(const vec& or, const vec& dir) { o = or; d = dir; }
	vec origin() const { return o; }
	vec direction() const { return d; }
	vec pointAtParameter(float t) const { return o + t * d; }
};

#endif
