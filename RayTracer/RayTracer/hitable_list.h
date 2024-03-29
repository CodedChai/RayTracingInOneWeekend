#ifndef HITABLELISTH
#define HITABLELISTH

#include "hitable.h"

class hitable_list : public hitable {
public:
	hitable_list() {}
	hitable_list(hitable **l, int n) { list = l; list_size = n; }
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;
	virtual bool boundingBox(float t0, float t1, aabb & box) const;
	hitable **list;
	int list_size;
};

bool hitable_list::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
	hit_record temp_rec;
	bool hit_anything = false;
	double closest_so_far = t_max;
	for (int i = 0; i < list_size; i++) {
		if (list[i]->hit(r, t_min, closest_so_far, temp_rec)) {
			hit_anything = true;
			closest_so_far = temp_rec.t;
			rec = temp_rec;
		}
	}
	return hit_anything;
}

bool hitable_list::boundingBox(float t0, float t1, aabb& box) const {
	if (list_size < 1) 
		return false;
	aabb tempBox;
	bool firstTrue = list[0]->boundingBox(t0, t1, tempBox);
	if (!firstTrue)
		return false;
	else
		box = tempBox;

	for (int i = 1; i < list_size; i++) {
		if (list[0]->boundingBox(t0, t1, tempBox))
			box = surrounding_box(box, tempBox);
		else
			return false;
	}
	return true;
}

#endif