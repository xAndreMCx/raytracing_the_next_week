#pragma once

#include <stdint.h>
#include <pthread.h>

/**
 * @brief The value of pi.
 */
#define RT_PI 3.1415926535897932385

/**
 * @brief Normalization constant for 32-bit random integers (2^32).
 */
#define RT_UINT32_MAX 4294967296

/**
 * @brief Converts degrees to radians.
 *
 * This macro converts an angle in degrees to radians.
 *
 * @param x The angle in degrees.
 * @return The angle in radians.
 */
#define DEG_TO_RAD(x) ((x) * (RT_PI / 180.0))

/**
 * @brief Converts radians to degrees.
 *
 * This macro converts an angle in radians to degrees.
 *
 * @param x The angle in radians.
 * @return The angle in degrees.
 */
#define RAD_TO_DEG(x) ((x) * (180.0 / RT_PI))

/**
 * @brief Thread-local random number generator state.
 *
 * Each thread has its own private instance of this variable to ensure
 * thread-safe random number generation without global locks.
 */
extern _Thread_local unsigned int rng_state;

/**
 * @brief Seeds the thread-local RNG.
 *
 * This should be called once at the start of each worker thread 
 * using a unique value (like the thread ID).
 *
 * @param seed The initial seed value.
 */
void seed_thread_rng(unsigned int seed);

/**
 * @brief Generates a random double between min and max.
 *
 * This function generates a random double value in the range [min, max).
 * It uses the thread-local rng_state.
 *
 * @param min The minimum value.
 * @param max The maximum value.
 * @return A random double value between min and max.
 */
double rand_double(double min, double max);

/**
 * @brief Generates a random int between min and max.
 *
 * This function generates a random double value in the range [min, max].
 * It uses the thread-local rng_state.
 *
 * @param min The minimum value.
 * @param max The maximum value.
 * @return A random int value between min and max.
 */
int rand_int(int min, int max);

/**
 * @brief Converts a linear color value to gamma-corrected value.
 *
 * @param linear The linear color value.
 * @return The gamma-corrected color value.
 */
double linear_to_gamma(double linear);
