#pragma once

#include <stdbool.h>

#include "aabb.h"
#include "hittable.h"
#include "interval.h"
#include "ray.h"

/**
 * @struct hittable_list_t
 * @brief A structure to represent a list of hittable objects.
 */
typedef struct {
  hittable_t** hittables; // Array of pointers to hittable objects.
  unsigned int length;    // Current number of hittable objects in the list.
  unsigned int capacity;  // Maximum capacity of the list.
  aabb_t bbox;            // The bounding box of all the hittables.
} hittable_list_t;

/**
 * @brief Creates a hittable list with a specified capacity.
 *
 * @param capacity The maximum capacity of the list.
 * @return A hittable list with the specified capacity.
 */
hittable_list_t hittable_list_create(unsigned int capacity);

/**
 * @brief Adds a hittable object to the list.
 *
 * @param list Pointer to the hittable list.
 * @param hittable Pointer to the hittable object to add.
 */
void hittable_list_add(hittable_list_t* list, hittable_t* hittable);

/**
 * @brief Clears all hittable objects from the list.
 *
 * @param list Pointer to the hittable list.
 */
void hittable_list_clear(hittable_list_t* list);

/**
 * @brief Frees the memory allocated for the hittable list.
 *
 * @param list Pointer to the hittable list.
 */
void hittable_list_free(hittable_list_t* list);

/**
 * @brief Checks if a ray hits any object in the hittable list.
 *
 * @param list Pointer to the hittable list.
 * @param ray Pointer to the ray.
 * @param interval Pointer to the interval.
 * @param hit_record Pointer to the hit record to store intersection information.
 * @return True if the ray hits any object in the list, false otherwise.
 */
bool hittable_list_hit(hittable_list_t* list, ray_t* ray, interval_t* interval, hit_record_t* hit_record);
