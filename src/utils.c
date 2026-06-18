#include "utils.h"
#include <math.h>

_Thread_local unsigned int rng_state = 123456789;

/**
 * @brief Internal Xorshift32 algorithm.
 *
 * Operates on the calling thread's private rng_state.
 *
 * @return A pseudo-random 32-bit unsigned integer.
 */
static unsigned int xorshift32(void) {
  rng_state ^= rng_state << 13;
  rng_state ^= rng_state >> 17;
  rng_state ^= rng_state << 5;
  return rng_state;
}

void seed_thread_rng(unsigned int seed) {
  if (seed != 0) {
    rng_state = seed;
  }
}

double rand_double(double min, double max) {
  double random_unit = xorshift32() / (float)RT_UINT32_MAX;
  return min + (max - min) * random_unit;
}

int rand_int(int min, int max) {
  return (int)rand_double(min, max+1);
}

double linear_to_gamma(double linear) {
  // Standard Gamma 2.2 correction
  return (linear > 0) ? pow(linear, 1.0 / 2.2) : 0;
}
