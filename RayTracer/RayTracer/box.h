#ifndef BOXH
#define BOXH

#include "axisAlignedRectangle.h"
#include "hitable_list.h"

class box : public hitable {
public:
	box() {}
	box(const vec& p0, const vec& p1, material *ptr);

	virtual bool hit(const ray& r, float t0, float t1, hit_record& rec) const;
	virtual bool boundingBox(float t0, float t1, aabb& box) const {
		box = aabb(pmin, pmax);
		return true;
	}

	vec pmin, pmax;
	hitable *list_ptr;
};

box::box(const vec& p0, const vec& p1, material *ptr) {
	pmin = p0;
	pmax = p1;
	hitable **list = new hitable*[6];
	list[0] = new xyRect(p0.x(), p1.x(), p0.y(), p1.y(), p1.z(), ptr);
	list[1] = new flipNormals(new xyRect(p0.x(), p1.x(), p0.y(), p1.y(), p0.z(), ptr));
	list[2] = new xzRect(p0.x(), p1.x(), p0.z(), p1.z(), p1.y(), ptr);
	list[3] = new flipNormals(new xzRect(p0.x(), p1.x(), p0.z(), p1.z(), p0.y(), ptr));
	list[4] = new yzRect(p0.y(), p1.y(), p0.z(), p1.z(), p1.x(), ptr);
	list[5] = new flipNormals(new yzRect(p0.y(), p1.y(), p0.z(), p1.z(), p0.x(), ptr));
	list_ptr = new hitable_list(list, 6);
}

bool box::hit(const ray& r, float t0, float t1, hit_record& rec) const {
	return list_ptr->hit(r, t0, t1, rec);
}

#endif
