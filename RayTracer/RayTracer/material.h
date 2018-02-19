#ifndef MATERIALH
#define MATERIALH

#include "ray.h"
#include "hitable.h"

vec random_in_unit_sphere() {
	vec p;
	do {
		p = 2.0 * vec((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX) - vec(1, 1, 1);
	} while (p.squaredLength() >= 1.0);
	return p;
}

vec reflect(const vec& v, const vec& n) {
	return v - 2 * dot(v, n) * n;
}

class material {
public:
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec& attenuation, ray& scattered) const = 0;
};

class lambertian : public material {
public:
	lambertian(const vec& a) : albedo(a) {}
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec& attenuation, ray& scattered) const {
		vec target = rec.p + rec.normal + random_in_unit_sphere();
		scattered = ray(rec.p, target - rec.p);
		attenuation = albedo;
		return true;
	}

	vec albedo;
};

class metal : public material {
public:
	metal(const vec& a, float f) : albedo(a) { if (f < 1) fuzz = f; else fuzz = 1.0; }
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec& attenuation, ray& scattered) const {
		vec reflected = reflect(unitVector(r_in.direction()), rec.normal);
		scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere());
		attenuation = albedo;
		return (dot(scattered.direction(), rec.normal) > 0);
	}

	vec albedo;
	float fuzz;
};

#endif