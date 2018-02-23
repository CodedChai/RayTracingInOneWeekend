#ifndef CONSTANTMEDIUMH
#define CONSTANTMEDIUMH

#include "hitable.h"
#include "texture.h"
#include "material.h"
#include <float.h>

class constantMedium : public hitable {
public:
	constantMedium(hitable *b, float d, texture *a) : boundary(b), density(d) { phaseFunction = new isotropic(a); }
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;
	virtual bool boundingBox(float t0, float t1, aabb& box) const {
		return boundary->boundingBox(t0, t1, box);
	}
	hitable *boundary;
	float density;
	material *phaseFunction;
};

bool constantMedium::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
	hit_record rec1, rec2;
	if (boundary->hit(r, -FLT_MAX, FLT_MAX, rec1)) {
		if (boundary->hit(r, rec1.t + 0.0001, FLT_MAX, rec2)) {

			if (rec1.t < t_min)
				rec1.t = t_min;
			if (rec2.t > t_max)
				rec2.t = t_max;
			if (rec1.t >= rec2.t)
				return false;
			if (rec1.t < 0)
				rec1.t = 0;
			
			float distanceInsideBoundary = (rec2.t - rec1.t) * r.direction().length();
			float hitDistance = -(1 / density) * log((float)rand() / RAND_MAX);
			if (hitDistance < distanceInsideBoundary) {
				rec.t = rec1.t + hitDistance / r.direction().length();
				rec.p = r.pointAtParameter(rec.t);
				rec.normal = vec(1, 0, 0); // Arbitrary
				rec.mat_ptr = phaseFunction;
				return true;
			}
		}
	}
	return false;
}

#endif