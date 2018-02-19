#include <iostream>
#include <fstream>
#include "ray.h"

using namespace std;

bool hitSphere(const vec& center, float radius, const ray& r) {
	vec oc = r.origin() - center;
	float a = dot(r.direction(), r.direction());
	float b = 2.0 * dot(oc, r.direction());
	float c = dot(oc, oc) - radius * radius;
	float discriminant = b * b - 4 * a * c; 
	//return (discriminant > 0);
	//cout << a << b << c << endl;
	if (discriminant < 0) {
		return -1.0;
	}
	else {
		return (-b - sqrt(discriminant) ) / (2.0 * a);
	}
}

vec color(const ray& r) {
	float t = hitSphere(vec(0, 0, -1), 0.5, r);
	if (t) {
		//return vec(1, 0, 0);
	}
	if (t > 0.0) {
		vec normal = unitVector(r.pointAtParameter(t) - vec(0, 0, -1));
		return 0.5 * vec(normal.x() + 1, normal.y() + 1, normal.z() + 1);
	}
	//cout << " t < 0 " << endl;

	vec unitDirection = unitVector(r.direction());
	t = 0.5 * (unitDirection.y() + 1.0);
	return ((1.0 - t) * vec(1.0, 1.0, 1.0) + t * vec(0.5, 0.7, 1.0));
}

int main() {
	ofstream imgOut;
	imgOut.open("Normal.ppm");
	int width = 200;
	int height = 100;

	vec lowerLeftCorner(-2.0, -1.0, -1.0);
	vec horizontal(4.0, 0.0, 0.0);
	vec vertical(0.0, 2.0, 0.0);
	vec origin(0.0, 0.0, 0.0);

	imgOut << "P3\n" << width << " " << height << "\n255\n";
	for (int j = height - 1; j >= 0; j--) {
		for (int i = 0; i < width; i++) {
			float u = float(i) / float(width);
			float v = float(j) / float(height);

			ray r(origin, lowerLeftCorner + u * horizontal + v * vertical);

			vec col = color(r);
			int ir = int(255.99 * col[0]);
			int ig = int(255.99 * col[1]);
			int ib = int(255.99 * col[2]);
			imgOut << ir << " " << ig << " " << ib << "\n";
		}
	}
	imgOut.close();
	return 0;
}