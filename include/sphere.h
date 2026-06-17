#pragma once

#include "aabb.h"
#include "hittable.h"
#include "interval.h"
#include "material.h"
#include "ray.h"
#include "vec.h"

/**
 * @struct sphere_t
 * @brief A structure to represent a sphere in the scene.
 */
typedef struct {
  hittable_t base;
  ray_t center;
  double radius;
  material_t* material;
} sphere_t;

/**
 * @brief Creates a sphere with the specified center, radius, and material.
 *
 * @param center Center of the sphere.
 * @param radius Radius of the sphere.
 * @param material Pointer to the material of the sphere.
 * @return A sphere with the specified properties.
 */
sphere_t sphere_create(vec3_t center, double radius, material_t* material);

/**
 * @brief Creates a sphere with the specified center, radius, and material.
 *
 * @param center1 Center of the sphere at t=0.
 * @param center2 Center of the sphere at t=1.
 * @param radius Radius of the sphere.
 * @param material Pointer to the material of the sphere.
 * @return A sphere with the specified properties.
 */
sphere_t sphere_moving_create(vec3_t center1, vec3_t center2, double radius, material_t* material);

/**
 * @brief Checks if a ray hits the sphere.
 *
 * @param sphere Pointer to the sphere.
 * @param ray Pointer to the ray.
 * @param interval Pointer to the interval.
 * @param hit_record Pointer to the hit record to store intersection information.
 * @return True if the ray hits the sphere, false otherwise.
 */
bool sphere_hit(sphere_t* sphere, ray_t* ray, interval_t* interval, hit_record_t* hit_record);

/**
 * @brief Returns the sphere's bounding box.
 *
 * @param sphere Pointer to the sphere.
 * @return The bounding box of the sphere.
 */
aabb_t sphere_bounding_box(sphere_t* sphere);
