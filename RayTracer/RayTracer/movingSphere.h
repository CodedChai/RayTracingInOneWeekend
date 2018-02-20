#ifndef MOVINGSPHEREH
#define MOVINGSPHEREH

#include "hitable.h"

class movingSphere : public hitable {
public:
	movingSphere() { }
	movingSphere(vec cen0, vec cen1, float t0, float t1, float r, material *m) : center0(cen0), center1(cen1), time0(t0), time1(t1), radius(r), mat_ptr(m) {};

	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;
	virtual bool boundingBox(float t0, float t1, aabb& box) const;
	vec center(float time) const;

	vec center0, center1;
	float time0, time1;
	float radius;
	material *mat_ptr;
};

vec movingSphere::center(float time) const {
	return center0 + ((time - time0) / (time1 - time0))*(center1 - center0);
}

bool movingSphere::boundingBox(float t0, float t1, aabb& box) const {
	aabb box0(center(t0) - vec(radius, radius, radius), center(t0) + vec(radius, radius, radius));
	aabb box1(center(t1) - vec(radius, radius, radius), center(t1) + vec(radius, radius, radius));
	box = surrounding_box(box0, box1);
	return true;
}

bool movingSphere::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
    vec oc = r.origin() - center(r.time());
    float a = dot(r.direction(), r.direction());
    float b = dot(oc, r.direction());
    float c = dot(oc, oc) - radius*radius;
    float discriminant = b*b - a*c;
    if (discriminant > 0) {
        float temp = (-b - sqrt(discriminant))/a;
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.pointAtParameter(rec.t);
            rec.normal = (rec.p - center(r.time())) / radius;
            rec.mat_ptr = mat_ptr;
            return true;
        }
        temp = (-b + sqrt(discriminant))/a;
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.pointAtParameter(rec.t);
            rec.normal = (rec.p - center(r.time())) / radius;
            rec.mat_ptr = mat_ptr;
            return true;
        }
    }
    return false;
}

#endif