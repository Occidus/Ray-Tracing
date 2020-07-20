#ifndef CAMERAH
#define CAMERAH

#include "ray.h"

Vec3f random_in_unit_disk() {
  Vec3f p;
  do {
    p = 2.0 * Vec3f(drand48(), drand48(), 0) - Vec3f(1, 1, 0);
  } while (Dot(p, p) >= 1.0);
  return p;
}

class camera {
public:
  camera(Vec3f lookfrom, Vec3f lookat, Vec3f vup, float vfov, float aspect,
         float aperture, float focus_dist) {
    lens_radius = aperture / 2;
    float theta = ToRadians(vfov);
    float half_height = tan(theta / 2);
    float half_width = aspect * half_height;
    origin = lookfrom;
    w = (lookfrom - lookat).Normalized();
    u = vup.Cross(w).Normalized();
    v = w.Cross(u);
    lower_left_corner = origin - half_width * focus_dist * u -
                        half_height * focus_dist * v - focus_dist * w;
    // lower_left_corner = origin - half_width * u - half_height * v - w;
    horizontal = 2 * half_width * focus_dist * u;
    vertical = 2 * half_height * focus_dist * v;
  }
  ray get_ray(float s, float t) {
    Vec3f rd = lens_radius * random_in_unit_disk();
    Vec3f offset = u * rd.x + v * rd.y;
    return ray(origin + offset, lower_left_corner + s * horizontal +
                                    t * vertical - origin - offset);
  }

  Vec3f origin;
  Vec3f lower_left_corner;
  Vec3f horizontal;
  Vec3f vertical;
  Vec3f u, v, w;
  float lens_radius;
};

#endif