#pragma once

#include <stdbool.h>

#include "interval.h"
#include "ray.h"
#include "vec.h"

typedef struct {
  interval_t x;
  interval_t y;
  interval_t z;
} aabb_t;

/**
 * @brief Creates a AABB with with spesified x, y, and z intervals.
 *
 * @param x The x-axis interval.
 * @param y The y-axis interval.
 * @param z The z-axis interval.
 * @return The created AABB.
 */
aabb_t aabb_create(interval_t* x, interval_t* y, interval_t* z);

/**
 * @brief Creates a AABB with with spesified cornerns.
 *
 * @param a Conrner 1.
 * @param b Conrner 2.
 * @return The created AABB.
 */
aabb_t aabb_from_points_create(vec3_t* a, vec3_t* b);

/**
 * @brief Creates a AABB from two AABB.
 *
 * @param box1 AABB 1.
 * @param box2 AABB 2.
 * @return The created AABB.
 */
aabb_t aabb_from_boxes_create(aabb_t* box1, aabb_t* box2);

/**
 * @brief Creates an empty AABB
 */
aabb_t aabb_empty(void);

/**
 * @brief Get the interval for a certain axis.
 *
 * n == 0 -> x-axis.
 * n == 1 -> y-axis.
 * n == 2 -> z-axis.
 *
 * @param aabb The AABB.
 * @param n The index.
 * @return The axis interval.
 */
interval_t* aabb_axis_interval(aabb_t* aabb, int n);

/**
 * @brief Checks if a ray hits a AABB.
 *
 * @param aabb The AABB.
 * @param ray The ray.
 * @param interval ?.
 * @return True if the ray hits the AABB, otherwise false.
 */
bool aabb_hit(aabb_t* aabb, ray_t* ray, interval_t* interval);

/**
 * @brief Returns the index of the longest axis of the bounding box.
 *
 * @param aabb The AABB.
 * @return 0 for X, 1 for Y, 2 for Z.
 */
int aabb_longest_axis(aabb_t* aabb);
