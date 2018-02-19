#include <iostream>
#include <fstream>
#include "hitable_list.h"
#include "sphere.h"
#include "camera.h"
#include <time.h>
#include "material.h"
#include <omp.h>

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

hitable *randomScene() {
	int n = 500;
	hitable **list = new hitable*[n + 1];
	list[0] = new sphere(vec(0, -1000, 0), 1000, new lambertian(vec(0.5, 0.5, 0.5)));
	int i = 1;
#pragma omp parallel for
	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			float choose_mat = (float)rand() / RAND_MAX;
			vec center(a + 0.9*(float)rand() / RAND_MAX, 0.2, b + 0.9*(float)rand() / RAND_MAX);
			if ((center - vec(4, 0.2, 0)).length() > 0.9) {
				if (choose_mat < 0.5) {  // diffuse
					list[i++] = new sphere(center, 0.2, new lambertian(vec((float)rand() / RAND_MAX*(float)rand() / RAND_MAX, (float)rand() / RAND_MAX*(float)rand() / RAND_MAX, (float)rand() / RAND_MAX*(float)rand() / RAND_MAX)));
				}
				else if (choose_mat < 0.8) { // metal
					list[i++] = new sphere(center, 0.2,
						new metal(vec(0.5*(1 + (float)rand() / RAND_MAX), 0.5*(1 + (float)rand() / RAND_MAX), 0.5*(1 + (float)rand() / RAND_MAX)), 0.5*(float)rand() / RAND_MAX));
				}
				else {  // glass
					list[i++] = new sphere(center, 0.2, new dielectric(1.5));
				}
			}
		}
	}

	list[i++] = new sphere(vec(0, 1, 0), 1.0, new dielectric(1.5));
	list[i++] = new sphere(vec(-4, 1, 0), 1.0, new lambertian(vec(0.4, 0.2, 0.1)));
	list[i++] = new sphere(vec(4, 1, 0), 1.0, new metal(vec(0.7, 0.6, 0.5), 0.0));

	return new hitable_list(list, i);
}

int main() {
	
srand((unsigned)time(NULL));
	ofstream imgOut;
	imgOut.open("OpenMP Setup.ppm");
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

	world = randomScene();

	vec UP(0, 1, 0);
	vec lookFrom(-13, 2, 3);
	vec lookAt(0, 0, 0);
	float dist_to_focus = 10;
	float aperture = 0.1;
	float vFoV = 20;
	float aspect = float(width) / float(height);
	int pixels = width * height;
	vec* outCols = new vec[pixels]();

	camera cam(lookFrom, lookAt, UP, vFoV, aspect, aperture, dist_to_focus);
	imgOut << "P3\n" << width << " " << height << "\n255\n";
#pragma omp parallel for
	for (int j = height - 1; j >= 0; j--) {
		for (int i = 0; i < width; i++) {
			vec col(0.0, 0.0, 0.0);
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
			float ir = int(255.99 * col[0]);
			float ig = int(255.99 * col[1]);
			float ib = int(255.99 * col[2]);

			//imgOut << ir << " " << ig << " " << ib << "\n";

			outCols[i * height + j] = vec(ir, ig, ib);

			
		}
	}

	for (int j = height - 1; j >= 0; j--) {
		for (int i = 0; i < width; i++) {
			imgOut << outCols[i * height + j].r() << " " << outCols[i * height + j].g() << " " << outCols[i * height + j].b() << "\n";

		}
	}


	imgOut.close();
	return 0;
}