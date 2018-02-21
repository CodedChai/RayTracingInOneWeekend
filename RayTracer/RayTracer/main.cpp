#include <iostream>
#include <fstream>
#include "hitable_list.h"
#include "sphere.h"
#include "movingSphere.h"
#include "camera.h"
#include <time.h>
#include "material.h"
#include <omp.h>
#include "bvh.h"

using namespace std;

float startTime = 0.0;
float endTime = 1.0;

float myRand() {
	return ((float)rand() / RAND_MAX);
}

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

hitable *twoSpheres() {
	texture *checker = new checkerTexture(new constantTexture(vec(0.2, 0.3, 0.1)), new constantTexture(vec(1.0, 1.0, 1.0)));
	int n = 50;
	hitable **list = new hitable*[n + 1];
	list[0] = new sphere(vec(0, -10, 0), 10, new lambertian(checker));
	list[1] = new sphere(vec(0,  10, 0), 10, new metal(new constantTexture(vec(1.0,1.0,1.0)), 0.0));

	return new hitable_list(list, 2);
}

hitable *randomScene() {
	int n = 500;
	hitable **list = new hitable*[n + 1];
	list[0] = new sphere(vec(0, -1000, 0), 1000, new lambertian(new constantTexture(vec(0.5, 0.5, 0.5))));
	int i = 1;
#pragma omp parallel for
	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			float choose_mat = myRand();
			vec center(a + myRand(), 0.2, b + myRand());
			if ((center - vec(4, 0.2, 0)).length() > 0.9) {
				if (choose_mat < 0.1) {	// motion blur diffuse
					list[i++] = new movingSphere(center, center + vec(.1, .3, 0.1), 0.0, 1.0, 0.2, 
						new lambertian(vec(myRand(), myRand(), myRand())));
					//list[i++] = new sphere(center, 0.2, new lambertian(vec((myRand())*(myRand()), (myRand())*(myRand()), (myRand())*(myRand()))));
				}
				else if (choose_mat < 0.5) {  // diffuse
					list[i++] = new sphere(center, 0.2, new lambertian(vec(myRand(), myRand(), myRand())));
				}
				else if (choose_mat < 0.8) { // metal
					list[i++] = new sphere(center, 0.2,
						new metal(vec(0.5*(1 + myRand()), 0.5*(1 + myRand()), 0.5*(1 + myRand())), 0.4*myRand()));
				}
				else {  // glass
					list[i++] = new sphere(center, 0.2, new dielectric(1.5));
				}
			}
		}
	}

	list[i++] = new sphere(vec(0, 1, 0), 1.0, new dielectric(1.5));
	list[i++] = new sphere(vec(-4, 1, 0), 1.0, new lambertian((vec(0.4, 0.2, 0.1))));
	list[i++] = new sphere(vec(4, 1, 0), 1.0, new metal(vec(0.7, 0.6, 0.5), 0.0));

	//return new hitable_list(list, i);
	return new bvh_node(list, i, 0.0, 1.0);
}

int main() {
	//cout << time(NULL);
	srand((unsigned)time(NULL));
	ofstream imgOut;
	imgOut.open("TwoTexturedSpheres.ppm");
	int width = 1920;
	int height = 1080;
	int samples = 100;	// samples per pixel

	
	hitable *list[4];
	// Origin, radius, material(color)
	list[0] = new sphere(vec(0, 0, -1), 0.5, new lambertian(new constantTexture(vec(0.8, 0.3, 0.3))));
	//list[0] = new sphere(vec(0, 0, -1), 0.5, new lambertian(vec(0.8, 0.3, 0.3)));

	texture *checker = new checkerTexture(new constantTexture(vec(0.2, 0.3, 0.1)), new constantTexture(vec(0.9, 0.9, 0.9)));

	list[1] = new sphere(vec(0, -100.5, -1), 100, new lambertian(checker));
	list[2] = new sphere(vec(1, 0, -1), 0.5, new metal(vec(0.8, 0.6, 0.2), 0.3));
	list[3] = new sphere(vec(-1, 0, -1), 0.5, new dielectric(1.5));
	//hitable *world = new bvh_node(list, 4, 0.0, 1.0); 

	hitable *world = twoSpheres();
	//hitable *world = randomScene();

	vec UP(0, 1, 0);
	vec lookFrom(13, 2, 3);
	vec lookAt(0, 0, 0);
	float dist_to_focus = 10;
	float aperture = 0.0;
	float vFoV = 20;
	float aspect = float(width) / float(height);
	int pixels = width * height;
	vec* outCols = new vec[pixels]();

	camera cam(lookFrom, lookAt, UP, vFoV, aspect, aperture, dist_to_focus, 0.0, 1.0);
	imgOut << "P3\n" << width << " " << height << "\n255\n";
#pragma omp parallel for
	for (int j = height - 1; j >= 0; j--) {
		for (int i = 0; i < width; i++) {
			vec col(0.0, 0.0, 0.0);
			for (int s = 0; s < samples; s++) {
				float u = float(i + myRand()) / float(width);
				float v = float(j + myRand()) / float(height);
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