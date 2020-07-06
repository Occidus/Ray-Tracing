#include <iostream>
#include "linear.h"
#include "ray.h"

using namespace r3;

bool hit_sphere(const Vec3f& center, float radius, const ray& r) {
	Vec3f oc = r.origin() - center;
	float a = Dot(r.direction(), r.direction());
	float b = 2.0 * Dot(oc, r.direction());
	float c = Dot(oc, oc) - radius*radius;
	float discr = b*b - 4*a*c;
	return (discr > 0);
}

Vec3f color(const ray& r) {
	if (hit_sphere(Vec3f(0.0,0.0,-1.0), 0.5, r))
		return Vec3f(1.0, 0.0, 0.0);
	Vec3f unit_direction = Vec3f(r.direction().Normalized());
	float t = 0.5*(unit_direction.y + 1.0);
	return (1.0-t)*Vec3f(1.0, 1.0, 1.0) + t*Vec3f(0.5, 0.7, 1.0);
}

int main() {
	int nx = 200;
	int ny = 100;
	std::cout << "P3\n" << nx << " " << ny << "\n255\n";
	Vec3f lower_left_corner = Vec3f(-2.0, -1.0, -1.0);
	Vec3f horizontal = Vec3f(4.0, 0.0, 0.0);
	Vec3f vertical = Vec3f(0.0, 2.0, 0.0);
	Vec3f origin = Vec3f(0.0, 0.0, 0.0);
	for (int j = ny-1; j>= 0; j--){
		for (int i = 0; i< nx; i++){
			float u = float(i) / float(nx);
			float v = float(j) / float(ny);
			ray r(origin, lower_left_corner + u*horizontal + v*vertical);
			Vec3f col = color(r);
			int ir = int(255.99*col.x);
			int ig = int(255.99*col.y);
			int ib = int(255.99*col.z);

			std::cout << ir << " " << ig << " " << ib << "\n";
		}
	}
}