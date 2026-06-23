#pragma once

#include <stdbool.h>

#include "aabb.h"
#include "hittable.h"
#include "interval.h"
#include "material.h"
#include "vec.h"

/**
 * @struct quad_t
 * @brief A structure to represent a quad in the scene.
 */
typedef struct {
  hittable_t base;
  vec3_t Q;
  vec3_t u;
  vec3_t v;
  vec3_t w;
  vec3_t normal;
  double D;
  material_t* material;
  aabb_t bbox;
} quad_t;

quad_t quad_create(vec3_t Q, vec3_t u, vec3_t v, material_t* material);

/**
 * @brief Checks if a ray hits the quad.
 *
 * @param quad Pointer to the quad.
 * @param ray Pointer to the ray.
 * @param interval Pointer to the interval.
 * @param hit_record Pointer to the hit record to store intersection information.
 * @return True if the ray hits the quad, false otherwise.
 */
bool quad_hit(quad_t* quad, ray_t* ray, interval_t* interval, hit_record_t* hit_record);

/**
 * @brief Returns the quad's bounding box.
 *
 * @param quad Pointer to the quad.
 * @return The bounding box of the quad.
 */
aabb_t quad_bounding_box(quad_t* quad);

/**
 * @brief Sets the quad's bounding box.
 *
 * @param quad Pointer to the quad.
 */
void quad_set_bounding_box(quad_t* quad);

/**
 * @brief TODO
 *
 * @param a TODO.
 * @param b TODO.
 * @param hit_record Pointer to the hit record.
 * @return True if hit point is within the quad, false otherwise.
 */
bool quad_is_interior(double a, double b, hit_record_t* hit_record);
