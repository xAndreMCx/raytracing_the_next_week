#include "bvh.h"

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>

#include "aabb.h"
#include "hittable.h"

static int box_compare(const void* a, const void* b, int axis_idx) {
  hittable_t* a_hittable = *(hittable_t**)a;
  hittable_t* b_hittable = *(hittable_t**)b;

  aabb_t box_a = hittable_bounding_box(a_hittable);
  aabb_t box_b = hittable_bounding_box(b_hittable);

  interval_t* a_axis = aabb_axis_interval(&box_a, axis_idx);
  interval_t* b_axis = aabb_axis_interval(&box_b, axis_idx);

  if (a_axis->min < b_axis->min) {
    return -1;
  }
  if (a_axis->min > b_axis->min) {
    return 1;
  }
  return 0;
}

static int box_x_compare(const void* a, const void* b) { return box_compare(a, b, 0); }
static int box_y_compare(const void* a, const void* b) { return box_compare(a, b, 1); }
static int box_z_compare(const void* a, const void* b) { return box_compare(a, b, 2); }

bvh_node_t* bvh_node_create(hittable_list_t* list, size_t start, size_t end) {
  assert(list);
  bvh_node_t* node = malloc(sizeof(bvh_node_t));
  assert(node);

  node->base.type = HITTABLE_BVH_NODE;

  node->bbox = aabb_empty();
  for (size_t i = start; i < end; i++) {
    aabb_t object_bbox = hittable_bounding_box(list->hittables[i]);
    node->bbox = aabb_from_boxes_create(&node->bbox, &object_bbox);
  }

  int axis = aabb_longest_axis(&node->bbox);
  int (*comparator)(const void*, const void*) = (axis == 0)   ? box_x_compare
                                                : (axis == 1) ? box_y_compare
                                                              : box_z_compare;

  size_t object_span = end - start;

  if (object_span == 1) {
    node->left = list->hittables[start];
    node->right = list->hittables[start];
  } else if (object_span == 2) {
    node->left = list->hittables[start];
    node->right = list->hittables[start + 1];
  } else {
    qsort(&list->hittables[start], object_span, sizeof(hittable_t*), comparator);

    size_t mid = start + object_span / 2;

    node->left = (hittable_t*)bvh_node_create(list, start, mid);
    node->right = (hittable_t*)bvh_node_create(list, mid, end);
  }

  return node;
}

bool bvh_node_hit(bvh_node_t* node, ray_t* ray, interval_t* interval, hit_record_t* hit_record) {
  assert(node);
  assert(ray);
  assert(interval);
  assert(hit_record);

  if (!aabb_hit(&node->bbox, ray, interval)) {
    return false;
  }

  bool hit_left = hittable_hit(node->left, ray, interval, hit_record);
  interval_t right_interval = interval_create(interval->min, hit_left ? hit_record->t : interval->max);
  bool hit_right = hittable_hit(node->right, ray, &right_interval, hit_record);

  return hit_left || hit_right;
}

aabb_t bvh_node_bounding_box(bvh_node_t* node) {
  assert(node);
  return node->bbox;
}
