#ifndef HITABLEH
#define HITABLEH

#include "ray.h"

using namespace r3;

class material;

struct hit_record {
  float t; // distance along ray to hit point
  Vec3f p; // hit point
  Vec3f normal; // normal at hit point
  material *mat_ptr; // material of object hit
};

class hitable {
public:
  virtual bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const = 0;
};

#endif