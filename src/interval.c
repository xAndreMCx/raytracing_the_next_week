#include "interval.h"

interval_t interval_create(double min, double max) {
  interval_t result = {.min = min, .max = max};
  return result;
}

interval_t interval_from_intervals_create(interval_t* a, interval_t* b) {
  double min = a->min <= b->min ? a->min : b->min;
  double max = a->max >= b->max ? a->max : b->max;
  return interval_create(min, max);
}

bool interval_contains(interval_t* interval, double x) { return (interval->min <= x && x <= interval->max); }

bool interval_surrounds(interval_t* interval, double x) { return (interval->min < x && x < interval->max); }

double interval_clamp(interval_t* interval, double x) {
  if (x < interval->min) {
    return interval->min;
  }
  if (x > interval->max) {
    return interval->max;
  }
  return x;
}

interval_t interval_expand(interval_t* interval, double delta) {
  double padding = delta / 2;
  return interval_create(interval->min - padding, interval->max + padding);
}
