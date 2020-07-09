#ifndef TORUSH
#define TORUSH

#include "hitable.h"
#include "math.h"

using namespace r3;

class torus : public hitable {
public:
  torus() {}
  torus(Vec3f cen, float br, float lr, material *m)
      : center(cen), bigr(br), litr(lr), mat(m){};
  virtual bool hit(const ray &ra, float tmin, float tmax,
                   hit_record &rec) const;
  Vec3f center;
  float bigr;
  float litr;
  material *mat;
};

bool torus::hit(const ray &ra, float t_min, float t_max,
                hit_record &rec) const {
  bool out = false;
  // Matrix4f objFromWorld = obj.modelPose.GetMatrix4().Inverted();
  Vec3f p0 = ra.origin() - center;
  Vec3f l = ra.direction().Normalized();
  double R = bigr;
  double r = litr;
  double g = Dot(l, l);
  double h = 2 * Dot(l, p0);
  double k = Dot(p0, p0) + (R * R) - (r * r);
  // Coefficients
  double a = g * g;
  double b = 2 * g * h;
  double c = (2 * g * k) + (h * h);
  double d = 2 * h * k;
  double e = k * k;

  Vec3f l2 = Vec3f(l.x, 0.0f, l.z);
  Vec3f p2 = Vec3f(p0.x, 0.0f, p0.z);

  double m = Dot(l2, l2);
  double n = 2 * Dot(l2, p2);
  double p = Dot(p2, p2);

  c += m * -4 * R * R;
  d += n * -4 * R * R;
  e += p * -4 * R * R;

  double a3 = a * a * a;
  double a2 = a * a;
  double b4 = b * b * b * b;
  double b3 = b * b * b;
  double b2 = b * b;
  // double c4 = c * c * c * c;
  double c3 = c * c * c;
  double c2 = c * c;
  // double d4 = d * d * d * d;
  // double d3 = d * d * d;
  double d2 = d * d;
  // double e3 = e * e * e;
  // double e2 = e * e;

  double delta; /*= 256 * a3 * e3 - 192 * a2 * b * d * e2 - 128 * a2 * c2 * e2 +
  144 * a2 * c * d2 * e; delta += -27 * a2 * d4 + 144 * a * b2 * c * e2 - 6 * a
  * b2 * d2 * e - 80 * a * b * c2 * d * e; delta += 18 * a * b * c * d3 + 16 * a
  * c4 * e - 4 * a * c3 * d2 - 27 * b4 * e2;
  delta += 18 * b3 * c * d * e - 4 * b3 * d3 - 4 * b2 * c3 * e + b2 * c2 * d2;*/
  double P = 8 * a * c - 3 * b2;
  R = b3 + 8 * d * a2 - 4 * a * b * c;
  double delta0 = c2 - 3.0 * b * d + 12.0 * a * e;
  double delta1 = 2.0 * c3 - 9.0 * b * c * d + 27.0 * b2 * e + 27.0 * a * d2 -
                  72.0 * a * c * e;
  double D =
      64 * a3 * e - 16 * a2 * c2 + 16 * a * b2 * c - 16 * a2 * b * d - 3 * b4;
  p = (P / (8 * a2));
  double q = (R / (8 * a3));
  double y = (delta1 * delta1 - 4 * delta0 * delta0 * delta0);
  delta = y / -27;
  double qrt = sqrt(y);
  double Q = cbrt(((delta1 + qrt) / 2.0));
  double S;
  double fi = 0;
  if (delta > 0) {
    fi = acos((delta1) / (2 * sqrt(delta0 * delta0 * delta0)));
    S = 0.5f * sqrt(-(2.0f / 3.0f) * p +
                    (2.0f / (3.0f * a)) * sqrt(delta0) * cos(fi / 3.0f));
  } else {
    S = 0.5 * sqrt((-2.0 / 3.0) * p + (1.0 / (3.0 * a)) * (Q + delta0 / Q));
  }
  double rt0 = sqrt(-4 * S * S - 2 * p + (q / S));
  double rt1 = sqrt(-4 * S * S - 2 * p - (q / S));
  double z0 = -(b / (4 * a)) - S + 0.5 * rt0;
  double z1 = -(b / (4 * a)) - S - 0.5 * rt0;
  double z2 = -(b / (4 * a)) + S + 0.5 * rt1;
  double z3 = -(b / (4 * a)) + S - 0.5 * rt1;
  double z = MAXFLOAT;
  if (z0 <= z && z0 > t_min) {
    z = z0;
  }
  if (z1 <= z && z1 > t_min) {
    z = z1;
  }
  if (z2 <= z && z2 > t_min) {
    z = z2;
  }
  if (z3 <= z && z3 > t_min) {
    z = z3;
  }

  /*fprintf(stderr, "Delta1:%lf qrt:%lf\n", delta1, qrt);
  fprintf(stderr, "a:%lf delta0:%lf fi:%lf\n", a, delta0, fi);
  fprintf(stderr, "S:%lf p:%lf q:%lf Q:%lf\n", S, p, q, Q);
  fprintf(stderr, "rt0:%lf rt1:%lf\n", rt0, rt1);
  fprintf(stderr, "Delta:%lf P:%lf D:%lf z:%lf\n", delta, P, D, z);
  fprintf(stderr, "a:%lf b:%lf c:%lf d:%lf e:%lf\n", a, b, c, d, e);
  //fprintf(stderr, "z0:%lf z1:%lf z2:%lf z3:%lf\t\tz:%lf\n", z0, z1, z2, z3,
  z);*/

  if (delta < 0) {
    out = true;
  }

  if (delta > 0) {
    if (P < 0 && D < 0) {
      out = true;
    } else {
      out = false;
    }
  }

  if (delta == 0) {
    if (P < 0 && D < 0 && delta0 != 0) {
      out = true;
    }
    if (D > 0 || (P > 0 && (D != 0 || R != 0))) {
      out = true;
    }
    if (delta0 == 0 && D != 0) {
      out = true;
    }
    if (D == 0) {
      if (P < 0) {
        out = true;
      }
      if (P > 0 && R == 0) {
        out = true;
      }
      if (delta0 == 0) {
        out = true;
      }
    }
  }

  if (z == MAXFLOAT) {
    return false;
  }

  if (out) {
    rec.t = z;
    rec.p = p0 + l * z;
    float u = atan2(rec.p.z, rec.p.x); // Radians
    rec.normal = Quaternionf(Vec3f(0, 1, 0), u) * rec.p;
    rec.normal.x -= bigr;
    float v = atan2(rec.normal.y, rec.normal.x); // Radians
    rec.normal = (Quaternionf(Vec3f(0, 1, 0), -u) * rec.normal).Normalized();
    rec.p = rec.p + center;
    rec.mat_ptr = mat;
  }

  return out;
}

#endif