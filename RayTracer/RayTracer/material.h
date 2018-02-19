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

bool refract(const vec& v, const vec& n, float ni_over_nt, vec& refracted) {
	vec uv = unitVector(v);
	float dt = dot(uv, n);
	float discriminant = 1.0 - ni_over_nt * ni_over_nt * (1 - dt*dt);
	if (discriminant > 0) {
		refracted = ni_over_nt * (uv - n * dt) - n * sqrt(discriminant);
		return true;
	}
	return false;
}

float schlick(float cosine, float ref_idx) {
	float r0 = (1 - ref_idx) / (1 + ref_idx);
	r0 = r0 * r0;
	return r0 + (1 - r0)*pow((1 - cosine), 5);
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

class dielectric : public material {
public:
	dielectric(float ri) : ref_idx(ri) {}

	virtual bool scatter(const ray& r_in, const hit_record& rec, vec& attenuation, ray& scattered) const {
		vec outward_normal;
		vec reflected = reflect(r_in.direction(), rec.normal);
		float ni_over_nt;
		attenuation = vec(1.0, 1.0, 1.0);
		vec refracted;
		float reflect_prob;
		float cosine;

		if (dot(r_in.direction(), rec.normal) > 0) {
			outward_normal = -rec.normal;
			ni_over_nt = ref_idx;
			cosine = ref_idx * dot(r_in.direction(), rec.normal) / r_in.direction().length();
		}
		else {
			outward_normal = rec.normal;
			ni_over_nt = 1.0 / ref_idx;
			cosine = -dot(r_in.direction(), rec.normal) / r_in.direction().length();
		}

		if (refract(r_in.direction(), outward_normal, ni_over_nt, refracted)) {
			reflect_prob = schlick(cosine, ref_idx);
		}
		else {
			reflect_prob = 1.0;
		}
		if ((float)rand() / RAND_MAX < reflect_prob) {
			scattered = ray(rec.p, reflected);
		}
		else {
			scattered = ray(rec.p, refracted);
		}
		return true;
	}

	float ref_idx;
};

#endif