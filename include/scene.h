#pragma once

#include "camera.h"
#include "hittable_list.h"

/**
 * @brief Function pointer type for scene building functions.
 */
typedef void (*scene_builder_fn)(hittable_list_t* world, camera_t* camera);

/**
 * @struct scene_registry_t
 * @brief Maps an integer ID and string name to a specific scene builder.
 */
typedef struct {
  int id;
  const char* name;
  scene_builder_fn build;
} scene_registry_t;

/**
 * @brief Retrieves a scene builder function by its ID.
 *
 * @param id The integer ID of the scene.
 * @return The corresponding scene_builder_fn, or the default scene if not found.
 */
scene_builder_fn scene_get_builder(int id);

void scene_bouncing_spheres(hittable_list_t* world, camera_t* camera);
void scene_checker_spheres(hittable_list_t* world, camera_t* camera);
void scene_earth(hittable_list_t* world, camera_t* camera);
void scene_perlin(hittable_list_t* world, camera_t* camera);
