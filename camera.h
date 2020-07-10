#ifndef CAMERAH
#define CAMERAH

#include "ray.h"

class camera {
public:
  camera(Vec3f lookfrom, Vec3f lookat, Vec3f vup, float vfov, float aspect) {
    Vec3f u, v, w;
    float theta = ToRadians(vfov);
    float half_height = tan(theta / 2);
    float half_width = aspect * half_height;
    origin = lookfrom;
    w = (lookfrom - lookat).Normalized();
    u = vup.Cross(w).Normalized();
    v = w.Cross(u);
    lower_left_corner = Vec3f(-half_width, -half_height, -1.0);
    lower_left_corner = origin - half_width * u - half_height * v - w;
    horizontal = 2 * half_width * u;
    vertical = 2 * half_height * v;
  }
  ray get_ray(float u, float v) {
    return ray(origin,
               lower_left_corner + u * horizontal + v * vertical - origin);
  }

  Vec3f origin;
  Vec3f lower_left_corner;
  Vec3f horizontal;
  Vec3f vertical;
};

#endif