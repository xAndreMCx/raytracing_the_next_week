#pragma once

#include <stdbool.h>

#include "interval.h"
#include "vec.h"
#include "ray.h"

typedef struct {
  interval_t x;
  interval_t y;
  interval_t z;
} aabb_t;

/**
 * @brief Creates a aabb with with spesified x, y, and z intervals.
 *
 * @param x The x-axis interval.
 * @param y The y-axis interval.
 * @param z The z-axis interval.
 * @return The created aabb.
 */
aabb_t aabb_create(interval_t* x, interval_t* y, interval_t* z);

/**
 * @brief Creates a aabb with with spesified cornerns.
 *
 * @param a Conrner 1.
 * @param b Conrner 2.
 * @return The created aabb.
 */
aabb_t aabb_from_points_create(vec3_t* a, vec3_t* b);

/**
 * @brief Get the interval for a certain axis.
 *
 * n == 0 -> x-axis.
 * n == 1 -> y-axis.
 * n == 2 -> z-axis.
 *
 * @param aabb The aabb.
 * @param n The index.
 * @return The axis interval.
 */
interval_t* aabb_axis_interval(aabb_t* aabb, int n);

/**
 * @brief Checks if a ray hits a aabb.
 *
 * @param aabb The aabb.
 * @param ray The ray.
 * @param interval ?.
 * @return True if the ray hits the aabb, otherwise false. 
 */
bool aabb_hit(aabb_t* aabb, ray_t* ray, interval_t* interval);
