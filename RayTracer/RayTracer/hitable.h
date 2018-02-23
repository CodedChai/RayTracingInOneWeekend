#ifndef HITABLEH
#define HITABLEH

#include "ray.h"
#include "aabb.h"
#define _USE_MATH_DEFINES
#include <math.h>
#ifndef M_PI
	#define M_PI 3.14159265358979323846
#endif

class material;

void getSphereUV(const vec& p, float& u, float& v) {
	float phi = atan2(p.z(), p.x());
	float theta = asin(p.y());
	u = 1 - (phi + M_PI) / (2.0 * M_PI);
	v = (theta + M_PI_2) / (M_PI);
}

struct hit_record {
	float t;
	float u;
	float v;
	vec p;
	vec normal;
	material *mat_ptr;
};

class hitable {
public:
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const = 0;
	virtual bool boundingBox(float t0, float t1, aabb& box) const = 0;
};

class flipNormals : public hitable {
public:
	flipNormals(hitable *p) : ptr(p) {}
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
		if (ptr->hit(r, t_min, t_max, rec)) {
			rec.normal = -rec.normal;
			return true;
		}
		else {
			return false;
		}
	}

	virtual bool boundingBox(float t0, float t1, aabb& box) const {
		return ptr->boundingBox(t0, t1, box);
	}

	hitable *ptr;
};

#endif
