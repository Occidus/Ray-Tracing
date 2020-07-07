#ifndef LAMBERTIANH
#define LAMBERTIANH

#include "material.h"
#include "ray.h"

using namespace r3;

class lambertian : public material {
public:
  lambertian(const Vec3f &a) : albedo(a) {}
  virtual bool scatter(const ray &r_in, const hit_record &rec,
                       Vec3f &attenuation, ray &scattered) const {
    Vec3f target = rec.p + rec.normal + drand48();
    scattered = ray(rec.p, target - rec.p);
    attenuation = albedo;
    return true;
  }

  Vec3f albedo;
};

#endif