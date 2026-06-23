#include "quad.h"
#include "aabb.h"
#include "hittable.h"
#include "interval.h"
#include "ray.h"
#include "vec.h"
#include <assert.h>

quad_t quad_create(vec3_t Q, vec3_t u, vec3_t v, material_t* material) {
  vec3_t n = vec3_cross(u, v);
  vec3_t normal = vec3_normalised(n);
  double D = vec3_dot(normal, Q);
  vec3_t w = vec3_div(n, vec3_dot(n, n));
  quad_t result = {.base = {HITTABLE_QUAD}, .Q = Q, .u = u, .v = v, .normal = normal, .D = D, .w = w, .material = material};
  quad_set_bounding_box(&result);
  return result;
}

bool quad_hit(quad_t* quad, ray_t* ray, interval_t* interval, hit_record_t* hit_record) {
  assert(quad);
  assert(ray);
  assert(interval);
  assert(hit_record);

  double denom = vec3_dot(quad->normal, ray->direction);
  if (fabs(denom) < 1e-4) {
    return false;
  }
  double t = (quad->D - vec3_dot(quad->normal, ray->origin)) / denom;
  if (!interval_contains(interval, t)) {
    return false;
  }

  vec3_t intersection = ray_point(ray, t);
  vec3_t planar_hit_point = vec3_sub(intersection, quad->Q); 
  double alpha = vec3_dot(quad->w, vec3_cross(planar_hit_point, quad->v));
  double beta = vec3_dot(quad->w, vec3_cross(quad->u, planar_hit_point));

  if (!quad_is_interior(alpha, beta, hit_record)) {
    return false; 
  }

  hit_record->t = t;
  hit_record->point = intersection;
  hit_record->material = quad->material;
  set_face_normal(hit_record, ray, &quad->normal);

  return true;
}

aabb_t quad_bounding_box(quad_t* quad) { return quad->bbox; }

void quad_set_bounding_box(quad_t* quad) {
  vec3_t diagonal_point = vec3_add(vec3_add(quad->Q, quad->u), quad->v);
  vec3_t top_point = vec3_add(quad->Q, quad->u);
  vec3_t right_point = vec3_add(quad->Q, quad->v);
  aabb_t diagonal1 = aabb_from_points_create(&quad->Q, &diagonal_point);
  aabb_t diagonal2 = aabb_from_points_create(&top_point, &right_point);
  aabb_t bbox = aabb_from_boxes_create(&diagonal1, &diagonal2);
  quad->bbox = bbox;
}

bool quad_is_interior(double a, double b, hit_record_t* hit_record) {
  interval_t unit = interval_create(0, 1);

  if (!interval_contains(&unit, a) || !interval_contains(&unit, b)) {
    return false;
  }

  hit_record->u = a;
  hit_record->v = b;
  return true;
}
