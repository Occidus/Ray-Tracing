#include <iostream>
#include "linear.h"
#include "sphere.h"
#include "torus.h"
#include "hitable_list.h"
#include "float.h"
#include "camera.h"
#include "material.h"
#include "lambertian.h"
#include "metal.h"

using namespace r3;

Vec3f color(const ray& r, hitable *world, int depth) {
	hit_record rec;
	if (world->hit(r, 0.001, MAXFLOAT, rec)) {
		ray scattered;
		Vec3f attenuation;
		if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
			return attenuation*color(scattered, world, depth+1);
		}
		else {
			return Vec3f(0,0,0);
		}
	}
	else {
		Vec3f unit_direction = r.direction().Normalized();
		float t = 0.5*(unit_direction.y + 1.0);
		return (1.0-t)*Vec3f(1.0, 1.0, 1.0) + t*Vec3f(0.5, 0.7, 1.0);
	}
}

int main() {
	int nx = 600;
	int ny = 300;
	int ns = 100;
	std::cout << "P3\n" << nx << " " << ny << "\n255\n";
	hitable *list[6];
	list[0] = new sphere(Vec3f(0,-0.25,-1), 0.25, new lambertian(Vec3f(0.8, 0.3, 0.3)));
	list[1] = new sphere(Vec3f(0,-100.5,-1), 100, new lambertian(Vec3f(0.8, 0.8, 0.0)));
	list[2] = new sphere(Vec3f(0.55,-0.25,-1), 0.25, new metal(Vec3f(0.8, 0.6, 0.2), 1.0));
	list[3] = new sphere(Vec3f(-0.55,-0.25,-1), 0.25, new metal(Vec3f(0.8, 0.8, 0.8), 0.3));
	list[4] = new torus(Vec3f(1.2, -0.4,-1), 0.15, 0.1, new metal(Vec3f(0.2, 0.6, 0.9), 1.0));
	list[5] = new torus(Vec3f(-1.3,-0.2,-1), 0.15, 0.1, new lambertian(Vec3f(0.3, 0.7, 0.2)));
	hitable *world = new hitable_list(list,6);
	camera cam;
	for (int j = ny-1; j>= 0; j--){
		for (int i = 0; i< nx; i++){
			bool trace = true;
			Vec3f col = Vec3f(0, 0, 0);
			for (int s=0; trace && s < ns; s++) {
				float u = float(i + drand48()) / float(nx);
				float v = float(j + drand48()) / float(ny);
				ray r = cam.get_ray(u, v);
				Vec3f p = r.point_at_parameter(2.0);
				col += color(r, world, 0); 
			}
			col /= float(ns);
			col = Vec3f( sqrt(col.x), sqrt(col.y), sqrt(col.z) );
			int ir = int(255.99*col.x);
			int ig = int(255.99*col.y);
			int ib = int(255.99*col.z);
			std::cout << ir << " " << ig << " " << ib << "\n";
		}
	}
}