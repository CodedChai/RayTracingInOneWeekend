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



#endif