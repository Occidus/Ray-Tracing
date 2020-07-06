#ifndef HITABLEH
#define HITABLEH

#include "ray.h"

using namespace r3;

struct hit_record {
  float t;
  Vec3f p;
  Vec3f normal;
};

class hitable {
public:
  virtual bool hit(const ray &r, float t_min, float t_max,
                   hit_record &rec) const = 0;
};

#endif