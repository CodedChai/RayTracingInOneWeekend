#include <iostream>
#include <fstream>
#include "hitable_list.h"
#include "sphere.h"
#include "movingSphere.h"
#include "camera.h"
#include <time.h>
#include "material.h"
#include "imageTexture.h"
#include <omp.h>
#include "axisAlignedRectangle.h"
#include "bvh.h"
#include "box.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define MAX_DEPTH 50

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
		vec emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);
		if (depth < MAX_DEPTH && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
			return emitted + attenuation * color(scattered, world, depth + 1);
			//return attenuation;
		}
		else {
			return emitted;
		}
	}
	else {
		return vec(0, 0, 0);
		// This is for the gradient 
		//vec unit_direction = unitVector(r.direction());
		//float t = 0.5 * (unit_direction.y() + 1.0);
		//return (1.0 - t) * vec(1.0, 1.0, 1.0) + t * vec(0.5, 0.7, 1.0);
	}
}

hitable *cornellBox() {
	hitable **list = new hitable*[8];
	int i = 0;
	material *red = new lambertian(new constantTexture(vec(0.65, 0.05, 0.05)));
	material *grey = new lambertian(new constantTexture(vec(0.73, 0.73, 0.73)));
	material *green = new lambertian(new constantTexture(vec(0.12, 0.45, 0.15)));
	material *light = new diffuseLight(new constantTexture(vec(15, 15, 15)));
	list[i++] = new flipNormals(new yzRect(0, 555, 0, 555, 555, green));
	list[i++] = new yzRect(0, 555, 0, 555, 0, red);
	list[i++] = new xzRect(113, 443, 127, 432, 540, light);
	//list[i++] = new flipNormals(new xzRect(200, 356, 214, 345, 400, light));
	list[i++] = new flipNormals(new xzRect(0, 555, 0, 555, 555, grey));
	list[i++] = new xzRect(0, 555, 0, 555, 0, grey);
	list[i++] = new flipNormals(new xyRect(0, 555, 0, 555, 555, grey));
	list[i++] = new box(vec(130, 0, 65), vec(295, 165, 230), grey);
	list[i++] = new box(vec(265, 0, 295), vec(430, 330, 460), grey);

	return new bvh_node(list, i, startTime, endTime);
}

hitable *simpleLight() {
	texture *perTex = new perlinTexture(2);
	hitable **list = new hitable*[5];
	list[0] = new sphere(vec(0, -1000, 0), 1000, new lambertian(perTex));
	list[1] = new sphere(vec(0, 2, 0), 2, new lambertian(perTex));
	//list[2] = new sphere(vec(0, 6, 0), 1, new diffuseLight(new constantTexture(vec(4,4,4))));
	list[2] = new xyRect(3, 5, 1, 3, -2, new diffuseLight(new constantTexture(vec(4, 0, 0))));
	list[3] = new xzRect(4, 6, 2, 4, 4, new diffuseLight(new constantTexture(vec(0, 4, 0))));
	list[4] = new yzRect(0, 4, 1, 4, -.5, new diffuseLight(new constantTexture(vec(0, 0, 6))));

	return new bvh_node(list, 5, startTime, endTime);
}

// For this to work, color has to return the attenuation only.
hitable *earth() {
	int nx, ny, nn;
	unsigned char *texData = stbi_load("earthmap.jpg", &nx, &ny, &nn, 0);
	material *mat = new lambertian(new imageTexture(texData, nx, ny));
	return new sphere(vec(0, 0, 0), 2, mat);
}

hitable *twoSpheres() {
	texture *checker = new checkerTexture(new constantTexture(vec(0.2, 0.3, 0.1)), new constantTexture(vec(1.0, 1.0, 1.0)));
	int n = 50;
	hitable **list = new hitable*[n + 1];
	list[0] = new sphere(vec(0, -10, 0), 10, new lambertian(checker));
	list[1] = new sphere(vec(0,  10, 0), 10, new metal(new constantTexture(vec(1.0,1.0,1.0)), 0.0));

	return new hitable_list(list, 2);
}

hitable *twoPerlinSpheres() {
	texture *perTex = new perlinTexture (2);
	hitable **list = new hitable*[2];
	list[0] = new sphere(vec(0, -1000, 0), 1000, new lambertian(perTex));
	list[1] = new sphere(vec(0, 2, 0), 2, new lambertian(perTex));
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
	return new bvh_node(list, i, startTime, endTime);
}

int main() {
	//cout << time(NULL);
	srand((unsigned)time(NULL));
	ofstream imgOut;
	imgOut.open("CornellBoxWithBoxes.ppm");
	int width = 1280;
	int height = 720;
	int samples = 3000;	// samples per pixel

	
	hitable *list[4];
	// Origin, radius, material(color)
	list[0] = new sphere(vec(0, 0, -1), 0.5, new lambertian(new constantTexture(vec(0.8, 0.3, 0.3))));
	//list[0] = new sphere(vec(0, 0, -1), 0.5, new lambertian(vec(0.8, 0.3, 0.3)));

	texture *checker = new checkerTexture(new constantTexture(vec(0.2, 0.3, 0.1)), new constantTexture(vec(0.9, 0.9, 0.9)));

	list[1] = new sphere(vec(0, -100.5, -1), 100, new lambertian(checker));
	list[2] = new sphere(vec(1, 0, -1), 0.5, new metal(vec(0.8, 0.6, 0.2), 0.3));
	list[3] = new sphere(vec(-1, 0, -1), 0.5, new dielectric(1.5));
	//hitable *world = new bvh_node(list, 4, 0.0, 1.0); 

	//hitable *world = twoSpheres();
	//hitable *world = randomScene();
	//hitable *world = twoPerlinSpheres();
	//hitable *world = earth();
	//hitable *world = simpleLight();
	hitable *world = cornellBox();

	vec UP(0, 1, 0);
	vec lookFrom(278, 278, -800);
	vec lookAt(278, 278, 0);
	float dist_to_focus = 13;
	float aperture = 0.0;
	float vFoV = 40;
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
			// The ffmin is because the emissive textures are too bright and recorded over 255
			outCols[i * height + j] = vec(ffmin(ir,255), ffmin(ig,255), ffmin(ib,255));

			
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