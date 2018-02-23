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

class translate : public hitable {
public:
	translate(hitable *p, const vec& displacement) : ptr(p), offset(displacement) {}
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;
	virtual bool boundingBox(float t0, float t1, aabb& box) const;
	hitable *ptr;
	vec offset;
};

bool translate::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
	ray translation(r.origin() - offset, r.direction(), r.time());
	if (ptr->hit(translation, t_min, t_max, rec)) {
		rec.p += offset;
		return true;
	}

	return false;
}

bool translate::boundingBox(float t0, float t1, aabb& box) const {
	if (ptr->boundingBox(t0, t1, box)) {
		box = aabb(box.min() + offset, box.max() + offset);
		return true;
	}

	return false;
}

class yRotate : public hitable {
public:
	yRotate(hitable *p, float angle);
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;
	virtual bool boundingBox(float t0, float t1, aabb& box) const {
		box = bbox;
		return hasBox;
	}
	hitable *ptr;
	float sinTheta, cosTheta;
	bool hasBox;
	aabb bbox;
};

yRotate::yRotate(hitable *p, float angle) : ptr(p) {
	float radians = (M_PI / 180.) * angle;
	sinTheta = sin(radians);
	cosTheta = cos(radians);
	hasBox = ptr->boundingBox(0, 1, bbox);
	vec min(FLT_MAX, FLT_MAX, FLT_MAX);
	vec max(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			for (int k = 0; k < 2; k++) {
				float x = i * bbox.max().x() + (1 - i) * bbox.min().x();
				float y = j * bbox.max().y() + (1 - j) * bbox.min().y();
				float z = k * bbox.max().z() + (1 - k) * bbox.min().z();
				float newx = cosTheta * x + sinTheta * z;
				float newz = -sinTheta * x + cosTheta * z;
				vec tester(newx, y, newz);
				for (int c = 0; c < 3; c++) {
					if (tester[c] > max[c])
						max[c] = tester[c];
					if (tester[c] < min[c])
						min[c] = tester[c];
				}
			}
		}
	}
	bbox = aabb(min, max);
}

bool yRotate::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
	vec origin = r.origin();
	vec direction = r.direction();
	origin[0] = cosTheta * r.origin().x() - sinTheta * r.origin().z();
	origin[2] = sinTheta * r.origin().x() + cosTheta * r.origin().z();
	direction[0] = cosTheta * r.direction().x() - sinTheta * r.direction().z();
	direction[2] = sinTheta * r.direction().x() + cosTheta * r.direction().z();
	ray rotation(origin, direction, r.time());
	if (ptr->hit(rotation, t_min, t_max, rec)) {
		vec p = rec.p;
		vec normal = rec.normal;
		p[0] = cosTheta * rec.p[0] + sinTheta * rec.p[2];
		p[2] = -sinTheta * rec.p[0] + cosTheta * rec.p[2];
		normal[0] = cosTheta * rec.normal[0] + sinTheta * rec.normal[2];
		normal[2] = -sinTheta * rec.normal[0] + cosTheta * rec.normal[2];
		rec.p = p;
		rec.normal = normal;
		return true;
	}
	return false;
}
#endif
