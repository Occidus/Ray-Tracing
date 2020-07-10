#ifndef METALH
#define METALH

Vec3f reflect(const Vec3f &v, const Vec3f &n) { return v - 2 * Dot(v, n) * n; };

class metal : public material {
public:
  metal(const Vec3f &a, float f) : albedo(a) {
    if (f < 1)
      fuzz = f;
    else
      fuzz = 1;
  }
  virtual bool scatter(const ray &r_in, const hit_record &rec,
                       Vec3f &attenuation, ray &scattered) const {
    Vec3f reflected = reflect(r_in.direction().Normalized(), rec.normal);
    scattered = ray(rec.p, reflected + fuzz * drand48());
    attenuation = albedo;
    return (Dot(scattered.direction(), rec.normal) > 0);
  }
  Vec3f albedo;
  float fuzz;
};

#endif