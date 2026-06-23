#pragma once

#include <stdbool.h>

#include "aabb.h"
#include "interval.h"
#include "ray.h"
#include "vec.h"

/**
 * @struct hit_record_t
 * @brief A structure to store information about a ray and object intersection.
 */
typedef struct {
  vec3_t point;      // The point of intersection.
  vec3_t normal;     // The normal at the intersection point.
  void* material;    // Pointer to the material of the object.
  double t;          // The parameter t of the ray at the point of intersection.
  double u;          // The U surface coordinate of the ray-object intersection.
  double v;          // The V surface coordinate of the ray-object intersection.
  bool front_facing; // Whether the intersection is front-facing.
} hit_record_t;

/**
 * @enum hittable_type_t
 * @brief An enumeration of different types of hittable objects.
 */
typedef enum { HITTABLE_UNKOWN = -1, HITTABLE_SPHERE = 0, HITTABLE_BVH_NODE = 1, HITTABLE_QUAD = 2 } hittable_type_t;

/**
 * @struct hittable_t
 * @brief A structure to represent a hittable object.
 */
typedef struct {
  hittable_type_t type;
} hittable_t;

/**
 * @brief Checks if a ray hits a hittable object.
 *
 * @param hittable Pointer to the hittable object.
 * @param ray Pointer to the ray.
 * @param interval Pointer to the interval.
 * @param hit_record Pointer to the hit record to store intersection information.
 * @return True if the ray hits the object, false otherwise.
 */
bool hittable_hit(hittable_t* hittable, ray_t* ray, interval_t* interval, hit_record_t* hit_record);

/**
 * @brief Returns the bounding box of a hittable object.
 *
 * @param hittable Pointer to the hittable object.
 * @return The bounding box.
 */
aabb_t hittable_bounding_box(hittable_t* hittable);

/**
 * @brief Sets the face normal of the hit record.
 *
 * @param hit_record Pointer to the hit record.
 * @param ray Pointer to the ray.
 * @param outward_normal Pointer to the outward normal vector.
 */
void set_face_normal(hit_record_t* hit_record, ray_t* ray, vec3_t* outward_normal);
