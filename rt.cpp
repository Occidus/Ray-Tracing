#include <iostream>
#include "sphere.h"
#include "torus.h"
#include "hitable_list.h"
#include "float.h"
#include "camera.h"
#include "material.h"
#include "lambertian.h"
#include "metal.h"
#include "dielectric.h"

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

hitable *random_scene() {
	int n = 500;
	hitable **list = new hitable*[n+1];
	list[0] = new sphere(Vec3f(0,-1000,0), 1000, new lambertian(Vec3f(0.5, 0.5, 0.5)));
	int i = 1;
	for (int a = -9; a < 9; a++) {
		for (int b = -9; b < 9; b++) {
			float choose_solid = drand48();
			float choose_mat = drand48();
			Vec3f center(a+ 0.9*drand48(),0.2,b+0.9*drand48());
			if ((center-Vec3f(4,0.2,0)).Length() > 0.9) {
				if (choose_solid < 0.65) { // sphere
					if (choose_mat < 0.8) { // diffuse
						list[i++] = new sphere(center, 0.2,
							new lambertian(Vec3f(drand48()*drand48(), drand48()*drand48(), drand48()*drand48())));
					}
					else if (choose_mat < 0.95) { // metal
						list[i++] = new sphere(center, 0.2,
							new metal(Vec3f(0.5*(1 + drand48()), 0.5*(1 + drand48()), 0.5*(1 + drand48())), 0.5*drand48()));
					}
					else { // glass
						list[i++] = new sphere(center, 0.2, new dielectric(1.5));
					}
				}
				else { // torus
					Vec3f tcenter(center.x,center.y-0.125,center.z);
					if (choose_mat < 0.8) { // diffuse
						list[i++] = new torus(tcenter, 0.125, 0.075,
							new lambertian(Vec3f(drand48()*drand48(), drand48()*drand48(), drand48()*drand48())));
					}
					else if (choose_mat < 0.95) { // metal
						list[i++] = new torus(tcenter, 0.125, 0.075,
							new metal(Vec3f(0.5*(1 + drand48()), 0.5*(1 + drand48()), 0.5*(1 + drand48())), 0.5*drand48()));
					}
					else { // glass
						list[i++] = new torus(tcenter, 0.125, 0.075, new dielectric(1.5));
					}
				}
			}
		}

		/*list[i++] = new sphere(Vec3f(0, 1, 0), 1.0, new dielectric(1.5));
		list[i++] = new sphere(Vec3f(-4, 1, 0), 1.0, new lambertian(Vec3f(0.4, 0.2, 0.1)));
		list[i++] = new sphere(Vec3f(4, 1, 0), 1.0, new metal(Vec3f(0.7, 0.6, 0.5), 0.0));

		return new hitable_list(list,i);*/

	}

	list[i++] = new sphere(Vec3f(0, 1, 0), 1.0, new dielectric(1.5));
	list[i++] = new sphere(Vec3f(-4, 1, 0), 1.0, new lambertian(Vec3f(0.4, 0.2, 0.1)));
	list[i++] = new sphere(Vec3f(4, 1, 0), 1.0, new metal(Vec3f(0.7, 0.6, 0.5), 0.0));

	return new hitable_list(list,i);
}

int main() {
	int nx = 150;
	int ny = 100;
	int ns = 100;
	std::cout << "P3\n" << nx << " " << ny << "\n255\n";
	
	/*hitable *list[10];
	list[0] = new sphere(Vec3f(0,-0.25,-1), 0.25, new lambertian(Vec3f(0.1, 0.2, 0.5)));
	list[1] = new sphere(Vec3f(0.5,-0.25,-1.5), 0.25, new lambertian(Vec3f(0.1, 0.2, 0.5)));
	list[2] = new sphere(Vec3f(-0.5,-0.25,-1.5), 0.25, new lambertian(Vec3f(0.1, 0.2, 0.5)));
	list[3] = new sphere(Vec3f(0,-100.5,-1), 100, new lambertian(Vec3f(0.8, 0.8, 0.0)));
	list[4] = new sphere(Vec3f(-0.55,-0.25,-1), 0.25, new dielectric(1.5));
	list[5] = new sphere(Vec3f(-0.55,-0.25,-1), -0.2, new dielectric(1.5));
	list[6] = new sphere(Vec3f(0.55,-0.25,-1), 0.25, new metal(Vec3f(0.8, 0.6, 0.2), 0.1));
	list[7] = new torus(Vec3f(1.2,-0.4,-1), 0.15, 0.1, new lambertian(Vec3f(0.1, 0.8, 0.3)));
	list[8] = new torus(Vec3f(-1.2, -0.4,-1), 0.15, 0.1, new metal(Vec3f(0.2, 0.6, 0.9), 0.1));
	list[9] = new torus(Vec3f(-0.9, -0.4,-1.5), 0.15, 0.1, new dielectric(1.01));
	hitable *world = new hitable_list(list,10);
	
	Vec3f lookfrom(0,0.75,0.5);
	Vec3f lookat(0,0,-1);
	Vec3f vup(0,1,0);
	float dist_to_focus = (lookfrom-lookat).Length();
	float aperture = 0.01;*/
	hitable *world = random_scene();
	Vec3f lookfrom(9,1.75,2.25);
	Vec3f lookat(0,0,-0.65);
	Vec3f vup(0,1,0);
	float dist_to_focus = (lookfrom-lookat).Length();
	float aperture = 0.1;
	camera cam(lookfrom, lookat, vup, 35, float(nx)/float(ny), aperture, dist_to_focus);
	for (int j = ny-1; j>= 0; j--){
		for (int i = 0; i< nx; i++){
			Vec3f col = Vec3f(0, 0, 0);
			for (int s=0;s < ns; s++) {
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