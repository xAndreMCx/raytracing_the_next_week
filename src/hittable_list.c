#include "hittable_list.h"
#include "aabb.h"
#include "hittable.h"

#include <assert.h>
#include <stdlib.h>

#define RESIZE_LIST_BY 16

static void hittable_list_resize(hittable_list_t* list, unsigned int resize_ammount) {
  list->hittables = realloc(list->hittables, (list->capacity + resize_ammount) * sizeof(hittable_t*));
  assert(list->hittables);
  list->capacity += resize_ammount;
}

hittable_list_t hittable_list_create(unsigned int capacity) {
  hittable_list_t result = {.hittables = calloc(capacity, sizeof(hittable_t*)), .length = 0, .capacity = capacity, .bbox = aabb_empty()};
  assert(result.hittables);
  return result;
}

void hittable_list_add(hittable_list_t* list, hittable_t* hittable) {
  assert(list);
  assert(hittable);

  list->length++;

  if (list->length > list->capacity) {
    hittable_list_resize(list, RESIZE_LIST_BY);
  }

  list->hittables[list->length - 1] = hittable;
  aabb_t hittable_aabb = hittable_bounding_box(hittable);
  list->bbox = aabb_from_boxes_create(&list->bbox, &hittable_aabb);
}

void hittable_list_clear(hittable_list_t* list) {
  assert(list);

  list->length = 0;
}

void hittable_list_free(hittable_list_t* list) {
  assert(list);

  free(list->hittables);
  list->capacity = 0;
  list->length = 0;
}

bool hittable_list_hit(hittable_list_t* list, ray_t* ray, interval_t* interval, hit_record_t* hit_record) {
  assert(ray);
  assert(interval);
  assert(hit_record);

  if (!list) {
    return false;
  }

  hit_record_t temp_record;
  bool hit_occurred = false;
  double closest_t_so_far = interval->max;

  for (unsigned int i = 0; i < list->length; i++) {
    interval_t temp_interval = interval_create(interval->min, closest_t_so_far);
    if (hittable_hit(list->hittables[i], ray, &temp_interval, &temp_record)) {
      hit_occurred = true;
      closest_t_so_far = temp_record.t;
      *hit_record = temp_record;
    }
  }
  return hit_occurred;
}
