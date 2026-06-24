#include "vec.h"

#include <math.h>
#include <stdio.h>

#include "utils.h"

void vec3_print(vec3_t a) { printf("vector = < %f, %f, %f >\n", a.x, a.y, a.z); }

vec3_t vec3_create(double x, double y, double z) {
  vec3_t result = {.x = x, .y = y, .z = z};
  return result;
}

vec3_t vec3_add(vec3_t a, vec3_t b) {
  vec3_t result = {.x = (a.x + b.x), .y = (a.y + b.y), .z = (a.z + b.z)};
  return result;
}

vec3_t vec3_sub(vec3_t a, vec3_t b) {
  vec3_t result = {.x = (a.x - b.x), .y = (a.y - b.y), .z = (a.z - b.z)};
  return result;
}

vec3_t vec3_negate(vec3_t a) {
  vec3_t result = {.x = -a.x, .y = -a.y, .z = -a.z};
  return result;
}

vec3_t vec3_scale(vec3_t a, double t) {
  vec3_t result = {.x = (a.x * t), .y = (a.y * t), .z = (a.z * t)};
  return result;
}

vec3_t vec3_div(vec3_t a, double t) {
  if (t == 0) {
    fprintf(stderr, "vec3_div div by 0 error.\n");
    return vec3_create(0, 0, 0);
  }
  return vec3_scale(a, (1 / t));
}

double vec3_dot(vec3_t a, vec3_t b) { return a.x * b.x + a.y * b.y + a.z * b.z; }

vec3_t vec3_cross(vec3_t a, vec3_t b) {
  vec3_t result = {.x = (a.y * b.z - a.z * b.y), .y = (a.z * b.x - a.x * b.z), .z = (a.x * b.y - a.y * b.x)};
  return result;
}

vec3_t vec3_hadamard(vec3_t a, vec3_t b) {
  vec3_t result = {.x = (a.x * b.x), .y = (a.y * b.y), .z = (a.z * b.z)};
  return result;
}

double vec3_length(vec3_t a) { return sqrt(vec3_length_squared(a)); }

double vec3_length_squared(vec3_t a) { return (a.x * a.x) + (a.y * a.y) + (a.z * a.z); }

vec3_t vec3_normalised(vec3_t a) {
  double length = vec3_length(a);
  if (length == 0) {
    fprintf(stderr, "vec3_normalised div by 0 error.\n");
    return vec3_create(0, 0, 0);
  }
  vec3_t result = {.x = (a.x / length), .y = (a.y / length), .z = (a.z / length)};
  return result;
}

vec3_t vec3_create_random(double min, double max) {
  vec3_t result = {.x = rand_double(min, max), .y = rand_double(min, max), .z = rand_double(min, max)};
  return result;
}

vec3_t vec3_create_random_unit(void) {
  while (1) {
    vec3_t p = vec3_create_random(-1, 1);
    if (vec3_length_squared(p) < 1) {
      return vec3_normalised(p);
    }
  }
}

vec3_t vec3_map(vec3_t a, double (*fn)(double)) {
  vec3_t result = {.x = fn(a.x), .y = fn(a.y), .z = fn(a.z)};
  return result;
}

bool vec3_near_zero(vec3_t a) {
  const double s = 1e-8;
  return (fabs(a.x) < s) && (fabs(a.y) < s) && (fabs(a.z) < s);
}

vec3_t vec3_reflect(vec3_t v, vec3_t normal) { return vec3_sub(v, vec3_scale(normal, 2 * vec3_dot(v, normal))); }

vec3_t vec3_refract(vec3_t uv, vec3_t normal, double etai_over_etat) {
  double cos_theta = fmin(vec3_dot(vec3_negate(uv), normal), 1.0);
  vec3_t r_perpendicular = vec3_scale(vec3_add(uv, vec3_scale(normal, cos_theta)), etai_over_etat);
  vec3_t r_parallel = vec3_scale(normal, -sqrt(fabs(1.0 - vec3_length_squared(r_perpendicular))));
  return vec3_add(r_perpendicular, r_parallel);
}

vec3_t vec3_random_in_unit_disk(void) {
  while (1) {
    vec3_t p = vec3_create(rand_double(-1, 1), rand_double(-1, 1), 0);
    if (vec3_length_squared(p) < 1) {
      return p;
    }
  }
}

color_t col_create(double r, double g, double b) {
  if (r < 0.0 || r > 1.0 || g < 0.0 || g > 1.0 || b < 0.0 || b > 1.0) {
    fprintf(stderr, "col_create warning: RGB values exceeds the range [0.0, 1.0].\n");
  }
  vec3_t color = {.r = r, .g = g, .b = b};
  return color;
}
