#ifndef PERLINH
#define PERLINH

inline float trilinearInterpolation(vec c[2][2][2], float u, float v, float w) {
	float uu = u*u*(3 - 2 * u);
	float vv = v*v*(3 - 2 * v);
	float ww = w*w*(3 - 2 * w);
	float accum = 0;
	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 2; j++)
			for (int k = 0; k < 2; k++) {
				vec weight_v(u - i, v - j, w - k);
				accum += (i*uu + (1 - i)*(1 - uu))*
					(j*vv + (1 - j)*(1 - vv))*
					(k*ww + (1 - k)*(1 - ww))*dot(c[i][j][k], weight_v);
			}
	return accum;
}


class perlin {
public:
	float noise(const vec& p) const {
		float u = p.x() - floor(p.x());
		float v = p.y() - floor(p.y());
		float w = p.z() - floor(p.z());
		int i = floor(p.x());
		int j = floor(p.y());
		int k = floor(p.z());
		vec c[2][2][2];
		for (int di = 0; di < 2; di++)
			for (int dj = 0; dj < 2; dj++)
				for (int dk = 0; dk < 2; dk++)
					c[di][dj][dk] = randVec[perm_x[(i + di) & 255] ^ perm_y[(j + dj) & 255] ^ perm_z[(k + dk) & 255]];
		return trilinearInterpolation(c, u, v, w);
	}

	float turb(const vec& p, int depth = 7) const {
		float accum = 0;
		vec temp = p;
		float weight = 1.0;
		for (int i = 0; i < depth; i++) {
			accum += weight * noise(temp);
			weight *= 0.5;
			temp *= 2.0;
		}
		return fabs(accum);
	}
	static vec *randVec;
	static int *perm_x;
	static int *perm_y;
	static int *perm_z;
};

static vec* perlinGenerate() {
	vec * p = new vec[256];
	for (int i = 0; i < 256; ++i) {
		p[i] = unitVector(vec(-1 + 2 * ((float)rand() / RAND_MAX), -1 + 2 * ((float)rand() / RAND_MAX), -1 + 2 * ((float)rand() / RAND_MAX)));
	}
	return p;
}

void permute(int *p, int n) {
	for (int i = n - 1; i > 0; i--) {
		int target = int(((float)rand() / RAND_MAX) * (i + 1));
		int temp = p[i];
		p[i] = p[target];
		p[target] = temp;
	}
	return;
}

static int* perlinGeneratePerm() {
	int * p = new int[256];
	for (int i = 0; i < 256; i++)
		p[i] = i;
	permute(p, 256);
	return p;
}

vec *perlin::randVec = perlinGenerate();
int *perlin::perm_x = perlinGeneratePerm();
int *perlin::perm_y = perlinGeneratePerm();
int *perlin::perm_z = perlinGeneratePerm();


#endif