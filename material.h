#ifndef MATERIALH
#define MATERIALH

#include "ray.h"

using namespace r3;

class material {
public:
  virtual bool scatter(const ray &r_in, const hit_record &rec,
                       Vec3f &attenuation, ray &scattered) const = 0;
};

#endif