#ifndef RAYH
#define RAYH
#include "vec.h"

class ray {
public:
	ray() { }
	ray(const vec& _o, const vec& _d, float ti = 0.0) { o = _o; d = _d; _time = ti; }
	vec origin() const { return o; }
	vec direction() const { return d; }
	float time() const { return _time; }
	vec pointAtParameter(float t) const { return o + t * d; }
	vec o;
	vec d;
	float _time;
};

#endif
