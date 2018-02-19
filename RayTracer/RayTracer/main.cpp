#include <iostream>
#include <fstream>
#include "hitable_list.h"
#include "sphere.h"
#include "camera.h"
#include <time.h>
#include "material.h"
using namespace std;

vec color(const ray& r, hitable *world, int depth) {
	hit_record rec;
	if (world->hit(r, 0.0001, FLT_MAX, rec)) {
		ray scattered;
		vec attenuation;
		if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
			return attenuation * color(scattered, world, depth + 1);
		}
		else {
			return vec(0, 0, 0);
		}
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
	imgOut.open("Depth of Field.ppm");
	int width = 1920;
	int height = 1080;
	int samples = 100;	// samples per pixel

	hitable *list[5];
	// Origin, radius, material(color)
	list[0] = new sphere(vec(0, 0, -1), 0.5, new lambertian(vec(0.8, 0.3, 0.3)));
	list[1] = new sphere(vec(0, -100.5, -1), 100, new lambertian(vec(0.8, 0.8, 0.0)));
	list[2] = new sphere(vec(1, 0, -1), 0.5, new metal(vec(0.8, 0.6, 0.2), 0.3));
	list[3] = new sphere(vec(-1, 0, -1), 0.5, new dielectric(1.5));
	list[4] = new sphere(vec(-1, 0, -1), -0.45, new dielectric(1.5));
	hitable *world = new hitable_list(list, 5);

	vec UP(0, 1, 0);
	vec lookFrom(-1, 1, 1);
	vec lookAt(0, 0, -1);
	float dist_to_focus = (lookFrom - lookAt).length();
	float aperture = 1.0;
	float vFoV = 90;
	float aspect = float(width) / float(height);

	camera cam(lookFrom, lookAt, UP, vFoV, aspect, aperture, dist_to_focus);
	imgOut << "P3\n" << width << " " << height << "\n255\n";
	for (int j = height - 1; j >= 0; j--) {
		for (int i = 0; i < width; i++) {
			vec col = vec(0.0, 0.0, 0.0);
			for (int s = 0; s < samples; s++) {
				float u = float(i + (float)rand() / RAND_MAX) / float(width);
				float v = float(j + (float)rand() / RAND_MAX) / float(height);
				ray r = cam.getRay(u, v);
				vec p = r.pointAtParameter(2.0);
				col += color(r, world, 0);
			}
			
			col /= float(samples);
			// Gamma2 color correction
			col = vec(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
			int ir = int(255.99 * col[0]);
			int ig = int(255.99 * col[1]);
			int ib = int(255.99 * col[2]);
			imgOut << ir << " " << ig << " " << ib << "\n";
		}
	}
	imgOut.close();
	return 0;
}