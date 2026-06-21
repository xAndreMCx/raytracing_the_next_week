#pragma once

#include "vec.h"

#define PERLIN_POINT_COUNT 256

/**
 * @struct perlin_t
 * @brief Holds the random floats and permutation arrays for noise generation.
 */
typedef struct {
  vec3_t random_vec[PERLIN_POINT_COUNT];
  int perm_x[PERLIN_POINT_COUNT];
  int perm_y[PERLIN_POINT_COUNT];
  int perm_z[PERLIN_POINT_COUNT];
} perlin_t;

/**
 * @brief Allocates and initializes a new Perlin noise generator.
 * @return Pointer to the new perlin_t instance.
 */
perlin_t* perlin_create(void);

/**
 * @brief Frees the memory associated with the Perlin noise generator.
 * @param perlin Pointer to the generator to free.
 */
void perlin_free(perlin_t* perlin);

/**
 * @brief Generates a blocky noise value at a given 3D point.
 * @param perlin Pointer to the generator.
 * @param p Pointer to the 3D point.
 * @return A random float value.
 */
double perlin_noise(const perlin_t* perlin, vec3_t* p);

/**
 * @brief TODO
 * @param perlin Pointer to the generator.
 * @param p Pointer to the 3D point.
 * @param depth TODO
 * @return TODO
 */
double perlin_turb(const perlin_t* perlin, vec3_t* p, int depth);
