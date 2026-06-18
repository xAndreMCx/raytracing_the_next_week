#include "sphere.h"

#include <assert.h>
#include <math.h>

#include "aabb.h"
#include "ray.h"
#include "vec.h"
#include "utils.h"

static void sphere_get_uv(vec3_t* point, double* u, double* v) {
  double theta = acos(point->y);
  double phi = atan2(-point->z, point->x) + RT_PI;

  *u = phi / (2 * RT_PI);
  *v = theta / RT_PI;
}

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
  sphere_get_uv(&outward_normal, &hit_record->u, &hit_record->v);

  return true;
}

// NOTE: could be optimized by check if the sphere is moving or not
aabb_t sphere_bounding_box(sphere_t* sphere) {
  assert(sphere);

  vec3_t radius_vec = vec3_create(sphere->radius, sphere->radius, sphere->radius);
  vec3_t corner1 = vec3_sub(ray_point(&sphere->center, 0), radius_vec);
  vec3_t corner2 = vec3_add(ray_point(&sphere->center, 0), radius_vec);
  aabb_t box1 = aabb_from_points_create(&corner1, &corner2);
  corner1 = vec3_sub(ray_point(&sphere->center, 1), radius_vec);
  corner2 = vec3_add(ray_point(&sphere->center, 1), radius_vec);
  aabb_t box2 = aabb_from_points_create(&corner1, &corner2);
  return aabb_from_boxes_create(&box1, &box2);
}
