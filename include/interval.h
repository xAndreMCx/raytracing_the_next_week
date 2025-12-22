#pragma once

#include <math.h>
#include <stdbool.h>

/**
 * @struct interval_t
 * @brief A structure to represent an interval with minimum and maximum values.
 */
typedef struct {
  double min;
  double max;
} interval_t;

/**
 * @brief Create an interval with specified min and max values.
 *
 * @param min The minimum value of the interval.
 * @param max The maximum value of the interval.
 * @return The created interval.
 */
interval_t interval_create(double min, double max);

/**
 * @brief Check if a value x is within the interval.
 *
 * @param interval The interval.
 * @param x The value to check.
 * @return True if the value is within the interval (inclusive), false otherwise.
 */
bool interval_contains(interval_t* interval, double x);

/**
 * @brief Check if the interval surrounds a value x.
 *
 * @param interval The interval.
 * @param x The value to check.
 * @return True if the value is within the interval (exclusive), false otherwise.
 */
bool interval_surrounds(interval_t* interval, double x);

/**
 * @brief Clamp a value x within the interval.
 *
 * @param interval The interval.
 * @param x The value to clamp.
 * @return The clamped value.
 */
double interval_clamp(interval_t* interval, double x);

/**
 * @brief Pad the interval with a certain amount.
 * 
 * @param interval The interval.
 * @param delta The value to pad the interval with.
 * @return The padded interval.
 */
interval_t interval_expand(interval_t* interval, double delta);

/**
 * @brief Macro for an empty interval.
 */
#define EMPTY_INTERVAL ((interval_t){.min = INFINITY, .max = -INFINITY})

/**
 * @brief Macro for a universal interval.
 *
 * The interval contains all numbers.
 */
#define UNIVERSE_INTERVAL ((interval_t){.min = -INFINITY, .max = INFINITY})
