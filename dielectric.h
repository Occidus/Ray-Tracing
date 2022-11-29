#ifndef DIELECTRICH
#define DIELECTRICH

bool refract(const Vec3f &v, const Vec3f &n, float ni_over_nt,
             Vec3f &refracted) {
  Vec3f uv = v.Normalized();
  float dt = Dot(uv, n);
  float discriminant = 1.0 - ni_over_nt * ni_over_nt * (1 - dt * dt);
  if (discriminant > 0) {
    refracted = ni_over_nt * (uv - n * dt) - n * sqrt(discriminant);
    return true;
  } else {
    return false;
  }
};

float schlick(float cosine, float ref_inx) {
  float r0 = (1 - ref_inx) / (1 + ref_inx);
  r0 = r0 * r0;
  return r0 + (1 - r0) * pow((1 - cosine), 5);
};

class dielectric : public material {
public:
  dielectric(float ri) : ref_idx(ri) {}
  virtual bool scatter(const ray &r_in, const hit_record &rec,
                       Vec3f &attenuation, ray &scattered) const {
    Vec3f outward_normal;
    Vec3f reflected = reflect(r_in.direction(), rec.normal);
    float ni_over_nt;
    attenuation = Vec3f(1.0, 1.0, 1.0);
    Vec3f refracted;
    float reflect_prob;
    float cosine;
    if (Dot(r_in.direction(), rec.normal) > 0) { // internal reflection
      outward_normal = -rec.normal;
      ni_over_nt = ref_idx;
      cosine = ref_idx * Dot(r_in.direction(), rec.normal) /
               r_in.direction().Length();
    } else { // reflection
      outward_normal = rec.normal;
      ni_over_nt = 1.0 / ref_idx;
      cosine = -Dot(r_in.direction(), rec.normal) / r_in.direction().Length();
    }
    if (refract(r_in.direction(), outward_normal, ni_over_nt, refracted)) {
      reflect_prob = schlick(cosine, ref_idx);
    } else {
      //scattered = ray(rec.p, reflected);
      reflect_prob = 1.0;
    }
    if (drand48() < reflect_prob) {
      scattered = ray(rec.p, reflected);
    } else {
      scattered = ray(rec.p, refracted);
    }
    return true;
  }

  float ref_idx;
};

#endif