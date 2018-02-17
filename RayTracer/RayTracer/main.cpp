#include <iostream>
#include <fstream>
#include "vec.h"

using namespace std;

int main() {
	ofstream imgOut;
	imgOut.open("Test.ppm");
	int width = 200;
	int height = 100;
	imgOut << "P3\n" << width << " " << height << "\n255\n";
	for (int j = height - 1; j >= 0; j--) {
		for (int i = 0; i < width; i++) {
			vec col(float(i) / float(width), float(j) / float(width), 0.2);
			int ir = int(255.99 * col[0]);
			int ig = int(255.99 * col[1]);
			int ib = int(255.99 * col[2]);
			imgOut << ir << " " << ig << " " << ib << "\n";
		}
	}
	imgOut.close();
	return 0;
}