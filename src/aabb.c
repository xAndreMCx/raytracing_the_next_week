#include "aabb.h"
#include "interval.h"

aabb_t aabb_create(interval_t* x, interval_t* y, interval_t* z) { return (aabb_t){.x = *x, .y = *y, .z = *z}; }

aabb_t aabb_from_points_create(vec3_t* a, vec3_t* b) {
  interval_t x = (a->x <= b->x) ? interval_create(a->x, b->x) : interval_create(b->x, a->x);
  interval_t y = (a->y <= b->y) ? interval_create(a->y, b->y) : interval_create(b->y, a->y);
  interval_t z = (a->z <= b->z) ? interval_create(a->z, b->z) : interval_create(b->z, a->z);
  return (aabb_t){.x = x, .y = y, .z = z};
}

interval_t* aabb_axis_interval(aabb_t* aabb, int n) {
  if (n == 1) {
    return &aabb->y;
  }
  if (n == 2) {
    return &aabb->z;
  }
  return &aabb->x;
}

bool aabb_hit(aabb_t* aabb, ray_t* ray, interval_t* interval) {
  for (int axis = 0; axis <= 3; axis++) {
    interval_t ax = *aabb_axis_interval(aabb, axis); 
    double ray_dir_inv = 1.0 / ray->direction.elements[axis];

    double t0 = (ax.min - ray->origin.elements[axis]) * ray_dir_inv;
    double t1 = (ax.max - ray->origin.elements[axis]) * ray_dir_inv;

    if (t0 < t1) {
      if (t0 > interval->min) {
        interval->min = t0; 
      }
      if (t1 < interval->max) {
        interval->max = t1; 
      }
    } else {
      if (t1 > interval->min) {
        interval->min = t1; 
      }
      if (t0 < interval->max) {
        interval->max = t0; 
      }

    }
    if (interval->max <= interval->min) {
      return false;
    }
  }

  return true;
}
