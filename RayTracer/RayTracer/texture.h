#ifndef TEXTUREH
#define TEXTUREH

#include "perlin.h"

class texture {
public:
	virtual vec value(float u, float v, const vec& p) const = 0;
};

class constantTexture : public texture {
public:
	constantTexture() { }
	constantTexture(vec c) : color(c) { }
	virtual vec value(float u, float v, const vec& p) const {
		return color;
	}
	vec color;
};

class checkerTexture : public texture {
public:
	checkerTexture() { }
	checkerTexture(texture *t0, texture *t1) : tex0(t0), tex1(t1) { }

	virtual vec value(float u, float v, const vec& p) const {
		float sineAtP = sin(10 * p.x()) * sin(10 * p.y()) * sin(10 * p.z());
		if (sineAtP < 0.0)
			return tex0->value(u, v, p);
		else
			return tex1->value(u, v, p);
		
	}

	texture *tex0;
	texture *tex1;
};

class perlinTexture : public texture {
public:
	perlinTexture() { scale = 1; }
	perlinTexture(float sc) : scale(sc) {}
	virtual vec value(float u, float v, const vec& p) const {
		//return vec(1, 1, 1) * noise.noise(scale * p);
		//return vec(1, 1, 1) * 0.5 * (1 + noise.turb(scale * p));
		//return vec(1, 1, 1) * noise.turb(scale * p);
		return vec(1, 1, 1)*0.5*(1 + sin(scale*p.z() + 5 * noise.turb(scale * p)));
	}

	perlin noise;
	float scale;
};


#endif