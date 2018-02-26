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
#include "constantMedium.h"
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
	material *light = new diffuseLight(new constantTexture(vec(10, 10, 10)));
	list[i++] = new flipNormals(new yzRect(0, 555, 0, 555, 555, green));
	list[i++] = new yzRect(0, 555, 0, 555, 0, red);
	list[i++] = new xzRect(113, 443, 127, 432, 554, light);
	//list[i++] = new flipNormals(new xzRect(200, 356, 214, 345, 400, light));
	list[i++] = new flipNormals(new xzRect(0, 555, 0, 555, 555, grey));
	list[i++] = new xzRect(0, 555, 0, 555, 0, grey);
	list[i++] = new flipNormals(new xyRect(0, 555, 0, 555, 555, grey));
	list[i++] = new translate(new yRotate(new box(vec(0, 0, 0), vec(165, 165, 165), grey), -18), vec(130, 0, 65));
	list[i++] = new translate(new yRotate(new box(vec(0, 0, 0), vec(165, 330, 165), grey), 15), vec(265, 0, 195));

	return new bvh_node(list, i, startTime, endTime);
}

hitable *cornellBoxSmoke() {
	hitable **list = new hitable*[8];
	int i = 0;
	material *red = new lambertian(new constantTexture(vec(0.65, 0.05, 0.05)));
	material *grey = new lambertian(new constantTexture(vec(0.73, 0.73, 0.73)));
	material *green = new lambertian(new constantTexture(vec(0.12, 0.45, 0.15)));
	material *light = new diffuseLight(new constantTexture(vec(10, 10, 10)));
	list[i++] = new flipNormals(new yzRect(0, 555, 0, 555, 555, green));
	list[i++] = new yzRect(0, 555, 0, 555, 0, red);
	list[i++] = new xzRect(113, 443, 127, 432, 554, light);
	//list[i++] = new flipNormals(new xzRect(200, 356, 214, 345, 400, light));
	list[i++] = new flipNormals(new xzRect(0, 555, 0, 555, 555, grey));
	list[i++] = new xzRect(0, 555, 0, 555, 0, grey);
	list[i++] = new flipNormals(new xyRect(0, 555, 0, 555, 555, grey));
	hitable *box0 = new translate(new yRotate(new box(vec(0, 0, 0), vec(165, 165, 165), grey), -18), vec(130, 0, 65));
	hitable *box1 = new translate(new yRotate(new box(vec(0, 0, 0), vec(165, 330, 165), grey), 15), vec(265, 0, 195));
	list[i++] = new constantMedium(box0, 0.007, new constantTexture(vec(1.0, 1.0, 1.0)));
	list[i++] = new constantMedium(box1, 0.013, new constantTexture(vec(0.0, 0.0, 0.0)));

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

hitable *book2Final() {
	int numBoxes = 20;
	float halfSceneWidth = 1000;
	float boxWidth = (2 * halfSceneWidth) / numBoxes;
	hitable **list = new hitable*[30];
	hitable **boxList = new hitable*[10000];
	hitable **sphereList = new hitable*[10000];
	material *white = new lambertian(new constantTexture(vec(0.73, 0.73, 0.73)));
	material *ground = new lambertian(new constantTexture(vec(0.48, 0.83, 0.53)));
	material *light = new diffuseLight(new constantTexture(vec(7, 7, 7)));
	int nx, ny, nn;
	unsigned char *texData = stbi_load("earthmap.jpg", &nx, &ny, &nn, 0);
	int numSpheres = 1000;
	vec center(400, 400, 200);
	int listSize = 0;
	int sphereContainerSize = 165;
	int b = 0;
	for (int i = 0; i < numBoxes; i++) {
		for (int j = 0; j < numBoxes; j++) {
			float x0 = -halfSceneWidth + i * boxWidth;
			float z0 = -halfSceneWidth + j * boxWidth;
			float y0 = 0;
			float x1 = x0 + boxWidth;
			float y1 = 100 * (myRand() + 0.01);
			float z1 = z0 + boxWidth;
			boxList[b++] = new box(vec(x0, y0, z0), vec(x1, y1, z1), ground);
		}
	}

	list[listSize++] = new bvh_node(boxList, b, startTime, endTime);
	list[listSize++] = new xzRect(123, 423, 147, 412, 554, light);
	list[listSize++] = new movingSphere(center, center + vec(30, 0, 0), startTime, endTime, 50, new lambertian(new constantTexture(vec(0.7, 0.3, 0.1))));
	list[listSize++] = new sphere(vec(260, 400, 45), 50, new dielectric(1.5));
	list[listSize++] = new sphere(vec(0, 150, 145), 50, new metal(vec(0.8, 0.8, 0.9), 10.0));
	list[listSize++] = new sphere(vec(105, 150, 145), 50, new metal(vec(0.7, 0.6, 0.5), 0.1));

	// Create a fake subsurface reflection sphere
	hitable *boundary = new sphere(vec(300, 150, 145), 70, new dielectric(1.5));
	list[listSize++] = boundary;
	list[listSize++] = new constantMedium(boundary, 0.2, new constantTexture(vec(0.2, 0.4, 0.9)));

	// create thin mist over everything
	boundary = new sphere(vec(0, 0, 0), 5000, new dielectric(1.5));
	list[listSize++] = new constantMedium(boundary, 0.0001, new constantTexture(vec(0.73, 0.73, 0.73)));
	material *emat = new lambertian(new imageTexture(texData, nx, ny));
	list[listSize++] = new sphere(vec(480, 200, 300), 100, emat);
	texture *perTex = new perlinTexture(0.1);
	list[listSize++] = new sphere(vec(200, 280, 300), 80, new lambertian(perTex));
#pragma omp parallel for
	for (int i = 0; i < numSpheres; i++) {
		sphereList[i] = new sphere(vec(sphereContainerSize * myRand(), sphereContainerSize * myRand(), sphereContainerSize * myRand()), 10, white);
	}

	list[listSize++] = new translate(new yRotate(new bvh_node(sphereList, numSpheres, startTime, endTime), 15), vec(-90, 270, 395));

	return new bvh_node(list, listSize, startTime, endTime);
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
	imgOut.open("Book 2 Final Product 2.ppm");
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

	//hitable *world = twoSpheres();
	//hitable *world = randomScene();
	//hitable *world = twoPerlinSpheres();
	//hitable *world = earth();
	//hitable *world = simpleLight();
	//hitable *world = cornellBox();
	//hitable *world = cornellBoxSmoke();
	hitable *world = book2Final();


	vec UP(0, 1, 0);
	vec lookFrom(375, 278, -800);
	vec lookAt(278, 278, 0);
	float dist_to_focus = 500;
	float aperture = 0.01;
	float vFoV = 40;
	float aspect = float(width) / float(height);
	int pixels = width * height;
	vec* outCols = new vec[pixels]();

	camera cam(lookFrom, lookAt, UP, vFoV, aspect, aperture, dist_to_focus, startTime, endTime);
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