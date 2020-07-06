#ifndef SPHEREH
#define SPHEREH

#include "hitable.h"

using namespace r3;

class sphere : public hitable {
public:
  sphere() {}
  sphere(Vec3f cen, float r) : center(cen), radius(r){};
  virtual bool hit(const ray &r, float tmin, float tmax, hit_record &rec) const;
  Vec3f center;
  float radius;
};

bool sphere::hit(const ray &r, float t_min, float t_max,
                 hit_record &rec) const {
  Vec3f oc = r.origin() - center;
  float a = Dot(r.direction(), r.direction());
  float b = Dot(oc, r.direction());
  float c = Dot(oc, oc) - radius * radius;
  float discr = b * b - a * c;
  if (discr > 0) {
    float temp = (-b - sqrt(b * b - a * c)) / a;
    if (temp < t_max && temp > t_min) {
      rec.t = temp;
      rec.p = r.point_at_parameter(rec.t);
      rec.normal = (rec.p - center) / radius;
      return true;
    }
    temp = (-b + sqrt(b * b - a * c)) / a;
    if (temp < t_max && temp > t_min) {
      rec.t = temp;
      rec.p = r.point_at_parameter(rec.t);
      rec.normal = (rec.p - center) / radius;
      return true;
    }
  }
  return false;
}

#endif