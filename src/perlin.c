#include "perlin.h"

#include <assert.h>
#include <math.h>
#include <stdlib.h>

#include "utils.h"
#include "vec.h"

static void permute(int* p, int n) {
  for (int i = n - 1; i > 0; i--) {
    int target = rand_int(0, i);
    int tmp = p[i];
    p[i] = p[target];
    p[target] = tmp;
  }
}

static void perlin_generate_perm(int* p) {
  for (int i = 0; i < PERLIN_POINT_COUNT; i++) {
    p[i] = i;
  }
  permute(p, PERLIN_POINT_COUNT);
}

static double perlin_interp(vec3_t c[2][2][2], double u, double v, double w) {
  double uu = u * u * (3 - 2 * u);
  double vv = v * v * (3 - 2 * v);
  double ww = w * w * (3 - 2 * w);
  double accum = 0.0;

  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      for (int k = 0; k < 2; k++) {
        vec3_t weight = vec3_create(u - i, v - j, w - k);
        accum += (i * uu + (1 - i) * (1 - uu)) * (j * vv + (1 - j) * (1 - vv)) * (k * ww + (1 - k) * (1 - ww)) *
                 vec3_dot(weight, c[i][j][k]);
      }
    }
  }

  return accum;
}

perlin_t* perlin_create(void) {
  perlin_t* p = malloc(sizeof(perlin_t));
  if (!p) {
    return NULL;
  }

  for (int i = 0; i < PERLIN_POINT_COUNT; i++) {
    p->random_vec[i] = vec3_create_random(-1.0, 1.0);
  }

  perlin_generate_perm(p->perm_x);
  perlin_generate_perm(p->perm_y);
  perlin_generate_perm(p->perm_z);

  return p;
}

void perlin_free(perlin_t* perlin) {
  if (perlin) {
    free(perlin);
  }
}

double perlin_noise(const perlin_t* perlin, vec3_t* p) {
  assert(perlin);

  double u = p->x - floor(p->x);
  double v = p->y - floor(p->y);
  double w = p->z - floor(p->z);

  int i = (int)floor(p->x);
  int j = (int)floor(p->y);
  int k = (int)floor(p->z);
  vec3_t c[2][2][2];

  for (int di = 0; di < 2; di++) {
    for (int dj = 0; dj < 2; dj++) {
      for (int dk = 0; dk < 2; dk++) {
        c[di][dj][dk] = perlin->random_vec[perlin->perm_x[(i + di) & 255] ^ perlin->perm_y[(j + dj) & 255] ^
                                           perlin->perm_z[(k + dk) & 255]];
      }
    }
  }

  return perlin_interp(c, u, v, w);
}

double perlin_turb(const perlin_t* perlin, vec3_t* p, int depth) {
  assert(perlin);

  double accum = 0.0;
  vec3_t temp_p = *p;
  double weight = 1.0;

  for (int i = 0; i < depth; i++) {
    accum += weight * perlin_noise(perlin, &temp_p);
    weight *= 0.5;
    temp_p = vec3_scale(temp_p, 2);
  }

  return fabs(accum);
}
