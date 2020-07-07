#ifndef METALH
#define METALH

#include "material.h"
#include "ray.h"

using namespace r3;

Vec3f reflect(const Vec3f &v, const Vec3f &n) { return v - 2 * Dot(v, n) * n; };

class metal : public material {
public:
  metal(const Vec3f &a) : albedo(a) {}
  virtual bool scatter(const ray &r_in, const hit_record &rec,
                       Vec3f &attenuation, ray &scattered) const {
    Vec3f reflected = reflect(r_in.direction().Normalized(), rec.normal);
    scattered = ray(rec.p, reflected);
    attenuation = albedo;
    return (Dot(scattered.direction(), rec.normal) > 0);
  }
  Vec3f albedo;
};

#endif