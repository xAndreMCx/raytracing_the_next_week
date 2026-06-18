#pragma once
#include <stdbool.h>
#include <stddef.h>

#include "aabb.h"
#include "hittable.h"
#include "hittable_list.h"

/**
 * @struct bvh_node_t
 * @brief Represents a node in a Bounding Volume Hierarchy (BVH) tree.
 */
typedef struct {
  hittable_t base;
  hittable_t* left;  // Pointer to the left child.
  hittable_t* right; // Pointer to the right child.
  aabb_t bbox;       // The bounding box for this node.
} bvh_node_t;

/**
 * @brief Creates a new bounding volume hierarchy (BVH) node.
 *
 * @param list Pointer to the list of hittable objects to be partitioned.
 * @param start The starting index in the hittable list.
 * @param end The ending index in the hittable list.
 * @return Pointer to the newly created BVH node.
 */
bvh_node_t* bvh_node_create(hittable_list_t* list, size_t start, size_t end);

/**
 * @brief Checks if a ray hits a BVH node or its children.
 *
 * @param node Pointer to the BVH node.
 * @param ray Pointer to the ray.
 * @param interval Pointer to the interval.
 * @param hit_record Pointer to the hit record to store intersection information.
 * @return True if the ray hits an object within the node, false otherwise.
 */
bool bvh_node_hit(bvh_node_t* node, ray_t* ray, interval_t* interval, hit_record_t* hit_record);

/**
 * @brief Returns the bounding box of a BVH node.
 *
 * @param node Pointer to the BVH node.
 * @return The bounding box.
 */
aabb_t bvh_node_bounding_box(bvh_node_t* node);

