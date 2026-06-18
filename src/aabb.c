#include "aabb.h"

#include <math.h>

#include "interval.h"

aabb_t aabb_create(interval_t* x, interval_t* y, interval_t* z) { return (aabb_t){.x = *x, .y = *y, .z = *z}; }

aabb_t aabb_from_points_create(vec3_t* a, vec3_t* b) {
  interval_t x = (a->x <= b->x) ? interval_create(a->x, b->x) : interval_create(b->x, a->x);
  interval_t y = (a->y <= b->y) ? interval_create(a->y, b->y) : interval_create(b->y, a->y);
  interval_t z = (a->z <= b->z) ? interval_create(a->z, b->z) : interval_create(b->z, a->z);
  return (aabb_t){.x = x, .y = y, .z = z};
}

aabb_t aabb_empty(void) {
  return (aabb_t){.x = {.min = INFINITY, .max = -INFINITY},
                  .y = {.min = INFINITY, .max = -INFINITY},
                  .z = {.min = INFINITY, .max = -INFINITY}};
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

aabb_t aabb_from_boxes_create(aabb_t* box1, aabb_t* box2) {
  interval_t x = interval_from_intervals_create(&box1->x, &box2->x);
  interval_t y = interval_from_intervals_create(&box1->y, &box2->y);
  interval_t z = interval_from_intervals_create(&box1->z, &box2->z);
  return aabb_create(&x, &y, &z);
}

bool aabb_hit(aabb_t* aabb, ray_t* ray, interval_t* interval) {
  interval_t ray_interval = *interval;
  for (int axis = 0; axis < 3; axis++) {
    interval_t ax = *aabb_axis_interval(aabb, axis);
    double ray_dir_inv = 1.0 / ray->direction.elements[axis];

    double t0 = (ax.min - ray->origin.elements[axis]) * ray_dir_inv;
    double t1 = (ax.max - ray->origin.elements[axis]) * ray_dir_inv;

    if (t0 < t1) {
      if (t0 > ray_interval.min) {
        ray_interval.min = t0;
      }
      if (t1 < ray_interval.max) {
        ray_interval.max = t1;
      }
    } else {
      if (t1 > ray_interval.min) {
        ray_interval.min = t1;
      }
      if (t0 < ray_interval.max) {
        ray_interval.max = t0;
      }
    }

    if (ray_interval.max <= ray_interval.min) {
      return false;
    }
  }

  return true;
}

int aabb_longest_axis(aabb_t* aabb) {
  double x_size = interval_size(&aabb->x);
  double y_size = interval_size(&aabb->y);
  double z_size = interval_size(&aabb->z);

  if (x_size > y_size) {
    return (x_size > z_size) ? 0 : 2;
  } else {
    return (y_size > z_size) ? 1 : 2;
  }
}
