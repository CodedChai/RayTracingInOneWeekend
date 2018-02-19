#ifndef RAYH
#define RAYH
#include "vec.h"

class ray {
public:
	ray() { }
	ray(const vec& _o, const vec& _d) { o = _o; d = _d; }
	vec origin() const { return o; }
	vec direction() const { return d; }
	vec pointAtParameter(float t) const { return o + t * d; }
	vec o;
	vec d;
};

#endif
