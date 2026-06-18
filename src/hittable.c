#include "hittable.h"

#include <assert.h>

#include "aabb.h"
#include "bvh.h"
#include "interval.h"
#include "sphere.h"

bool hittable_hit(hittable_t* hittable, ray_t* ray, interval_t* interval, hit_record_t* hit_record) {
  assert(hittable);
  assert(ray);
  assert(interval);
  assert(hit_record);

  switch (hittable->type) {
  case HITTABLE_SPHERE:
    return sphere_hit((sphere_t*)hittable, ray, interval, hit_record);

  case HITTABLE_BVH_NODE:
    return bvh_node_hit((bvh_node_t*)hittable, ray, interval, hit_record);

  default:

    return 0;
  }
}

aabb_t hittable_bounding_box(hittable_t* hittable) {
  assert(hittable);

  switch (hittable->type) {
  case HITTABLE_SPHERE:
    return sphere_bounding_box((sphere_t*)hittable);

  case HITTABLE_BVH_NODE:
    return bvh_node_bounding_box((bvh_node_t*)hittable);

  default:

    return aabb_empty();
  }
}

void set_face_normal(hit_record_t* hit_record, ray_t* ray, vec3_t* outward_normal) {
  assert(hit_record);
  assert(ray);
  assert(outward_normal);

  hit_record->front_facing = (vec3_dot(ray->direction, *outward_normal) < 0);
  hit_record->normal = (hit_record->front_facing) ? *outward_normal : vec3_negate(*outward_normal);
}
