#ifndef CAMERAH
#define CAMERAH

#include "ray.h"

class camera {
public:
  camera() {
    lower_left_corner = Vec3f(-2.0, -1.0, -1.0);
    horizontal = Vec3f(4.0, 0.0, 0.0);
    vertical = Vec3f(0.0, 2.0, 0.0);
    origin = Vec3f(0.0, 0.0, 0.0);
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