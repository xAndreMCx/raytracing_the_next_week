#pragma once

#include "vec.h"

/**
 * @struct ray_t
 * @brief Represents a ray in 3D space with an origin, a direction, and a time index between 0 and 1.
 */
typedef struct {
  vec3_t origin;
  vec3_t direction;
  double time;
} ray_t;

/**
 * @brief Computes the point along the ray at parameter t.
 *
 * This function calculates the point along the ray given a parameter t.
 * The point is computed as origin + t * direction.
 *
 * @param ray The ray.
 * @param t The parameter along the ray.
 * @return The point along the ray at parameter t.
 */
vec3_t ray_point(ray_t* ray, double t);
