#include <iostream>
#include <fstream>
#include "hitable_list.h"
#include "sphere.h"
#include "float.h"

using namespace std;

vec color(const ray& r, hitable *world) {
	hit_record rec;
	if (world->hit(r, 0.0, FLT_MAX, rec)) {
		return 0.5 * vec(rec.normal.x() + 1, rec.normal.y() + 1, rec.normal.z() + 1);
	}
	else {
		vec unit_direction = unitVector(r.direction());
		float t = 0.5 * (unit_direction.y() + 1.0);
		return (1.0 - t) * vec(1.0, 1.0, 1.0) + t * vec(0.5, 0.7, 1.0);
	}
}

int main() {
	ofstream imgOut;
	imgOut.open("Normal.ppm");
	int width = 400;
	int height = 300;

	vec lowerLeftCorner(-2.0, -1.0, -1.0);
	vec horizontal(4.0, 0.0, 0.0);
	vec vertical(0.0, 2.0, 0.0);
	vec origin(0.0, 0.0, 0.0);
	hitable *list[2];
	list[0] = new sphere(vec(0, 0, -1), 0.5);
	list[1] = new sphere(vec(0, -100.5, -1), 100);
	hitable *world = new hitable_list(list, 2);
	imgOut << "P3\n" << width << " " << height << "\n255\n";
	for (int j = height - 1; j >= 0; j--) {
		for (int i = 0; i < width; i++) {
			float u = float(i) / float(width);
			float v = float(j) / float(height);

			ray r(origin, lowerLeftCorner + u * horizontal + v * vertical);

			vec p = r.pointAtParameter(2.0);
			vec col = color(r, world);
			int ir = int(255.99 * col[0]);
			int ig = int(255.99 * col[1]);
			int ib = int(255.99 * col[2]);
			imgOut << ir << " " << ig << " " << ib << "\n";
		}
	}
	imgOut.close();
	return 0;
}