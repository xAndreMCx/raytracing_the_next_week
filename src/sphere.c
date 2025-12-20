#include "sphere.h"

#include <assert.h>

#include "math.h"
#include "ray.h"
#include "vec.h"

sphere_t sphere_create(vec3_t center, double radius, material_t* material) {
  sphere_t result = {.base = {HITTABLE_SPHERE},
                     .center = (ray_t){center, vec3_create(0, 0, 0), 0},
                     .radius = radius,
                     .material = material};
  return result;
}

sphere_t sphere_moving_create(vec3_t center1, vec3_t center2, double radius, material_t* material) {
  sphere_t result = {.base = {HITTABLE_SPHERE},
                     .center = (ray_t){center1, vec3_sub(center2, center1), 0},
                     .radius = radius,
                     .material = material};
  return result;
}

bool sphere_hit(sphere_t* sphere, ray_t* ray, interval_t* interval, hit_record_t* hit_record) {
  assert(sphere);
  assert(ray);
  assert(interval);
  assert(hit_record);

  vec3_t current_center = ray_point(&sphere->center, ray->time);
  vec3_t oc = vec3_sub(ray->origin, current_center);
  double a = vec3_length_squared(ray->direction);
  double half_b = vec3_dot(oc, ray->direction);
  double c = vec3_length_squared(oc) - (sphere->radius * sphere->radius);
  double discriminant = (half_b * half_b) - (a * c);

  if (discriminant < 0) {
    return false;
  }

  double sqrt_discriminant = sqrt(discriminant);

  // Find the nearest root that's in an acceptable range
  double root = (-half_b - sqrt_discriminant) / a;
  if (!interval_surrounds(interval, root)) {
    root = (-half_b + sqrt_discriminant) / a;
    if (!interval_surrounds(interval, root)) {
      return false;
    }
  }

  hit_record->t = root;
  hit_record->point = ray_point(ray, root);
  hit_record->material = sphere->material;
  vec3_t outward_normal = vec3_div(vec3_sub(hit_record->point, current_center), sphere->radius);
  set_face_normal(hit_record, ray, &outward_normal);
  return true;
}
