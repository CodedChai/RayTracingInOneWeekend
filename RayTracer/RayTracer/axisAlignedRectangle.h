#ifndef AARECTH
#define AARECTH

#include "hitable.h"

class xyRect : public hitable {
public:
	xyRect(){}
	xyRect(float _x0, float _x1, float _y0, float _y1, float _k, material *mat): x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k), mat_ptr(mat) {}

	virtual bool hit(const ray& r, float t0, float t1, hit_record& rec) const;
	virtual bool boundingBox(float t0, float t1, aabb& box) const {
		box = aabb(vec(x0, y0, k - .0001), vec(x1, y1, k + 0001));
		return true;
	}

	material *mat_ptr;
	float x0, x1, y0, y1, k;
};

bool xyRect::hit(const ray& r, float t0, float t1, hit_record& rec) const {
	float t = (k - r.origin().z()) / r.direction().z();
	if (t < t0 || t > t1)
		return false;
	
	float x = r.origin().x() + t*r.direction().x();
	float y = r.origin().y() + t*r.direction().y();
	if (x<x0 || x > x1 || y < y0 || y > y1)
		return false;

	rec.u = (x - x0) / (x1 / x0);
	rec.v = (y - y0) / (y1 / y0);
	rec.t = t;
	rec.mat_ptr = mat_ptr;
	rec.p = r.pointAtParameter(t);
	rec.normal = vec(0, 0, 1);
	return true;
}

#endif