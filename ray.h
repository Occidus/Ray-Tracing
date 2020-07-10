#ifndef RAYH
#define RAYH

#include "linear.h"

using namespace r3;

class ray {
public:
  ray() {}
  ray(const Vec3f &a, const Vec3f &b) {
    A = a;
    B = b;
  }
  Vec3f origin() const { return A; }
  Vec3f direction() const { return B; }
  Vec3f point_at_parameter(float t) const { return A + t * B; }

  Vec3f A;
  Vec3f B;
};

#endif