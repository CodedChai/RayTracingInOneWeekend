#include <iostream>
#include <fstream>
#include "hitable_list.h"
#include "sphere.h"
#include "camera.h"
#include <time.h>

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
	srand((unsigned)time(NULL));
	ofstream imgOut;
	imgOut.open("AntiAliasing.ppm");
	int height = 400;
	int width = height * 2;
	int samples = 100;	// samples per pixel
	hitable *list[2];
	list[0] = new sphere(vec(0, 0, -1), 0.5);
	list[1] = new sphere(vec(0, -100.5, -1), 100);
	hitable *world = new hitable_list(list, 2);
	camera cam;
	imgOut << "P3\n" << width << " " << height << "\n255\n";
	for (int j = height - 1; j >= 0; j--) {
		for (int i = 0; i < width; i++) {
			vec col = vec(0.0, 0.0, 0.0);
			for (int s = 0; s < samples; s++) {
				float u = float(i + (float)rand() / RAND_MAX) / float(width);
				float v = float(j + (float)rand() / RAND_MAX) / float(height);
				ray r = cam.getRay(u, v);
				vec p = r.pointAtParameter(2.0);
				col += color(r, world);
			}
			
			col /= float(samples);
			int ir = int(255.99 * col[0]);
			int ig = int(255.99 * col[1]);
			int ib = int(255.99 * col[2]);
			imgOut << ir << " " << ig << " " << ib << "\n";
		}
	}
	imgOut.close();
	return 0;
}