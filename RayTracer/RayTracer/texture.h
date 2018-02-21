#ifndef TEXTUREH
#define TEXTUREH

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


#endif