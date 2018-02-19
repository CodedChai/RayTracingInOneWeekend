#ifndef CAMERAH
#define CAMERAH
#define _USE_MATH_DEFINES
#include <math.h>
#include "ray.h"

class camera {
public:
	camera(vec lookFrom, vec lookAt, vec vup, float vfov, float aspect) {// vfov is top to bottom in degrees
		vec u, v, w;
		float theta = vfov * M_PI / 180.0;
		float half_height = tan(theta / 2);
		float half_width = aspect * half_height;
		origin = lookFrom;
		w = unitVector(lookFrom - lookAt);
		u = unitVector(cross(vup, w));
		v = cross(w, u);
		lower_left_corner = origin - half_width * u - half_height * v - w;
		horizontal = 2 * half_width * u;;
		vertical = 2 * half_height * v;
		
	}
	ray getRay(float u, float v) { return ray(origin, lower_left_corner + u*horizontal + v*vertical - origin); }

	vec origin;
	vec lower_left_corner;
	vec horizontal;
	vec vertical;
};

#endif