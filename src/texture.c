#include "texture.h"

#include <assert.h>
#include <math.h>
#include <stdlib.h>

#include "image.h"
#include "interval.h"
#include "vec.h"

color_t texture_value(texture_t* texture, double u, double v, vec3_t* p) {
  assert(texture);

  switch (texture->type) {
  case TEXTURE_SOLID: {
    return ((solid_texture_t*)texture)->albedo;
  }
  case TEXTURE_CHECKER: {
    checker_texture_t* checker = (checker_texture_t*)texture;

    int x_int = (int)floor(checker->inv_scale * p->x);
    int y_int = (int)floor(checker->inv_scale * p->y);
    int z_int = (int)floor(checker->inv_scale * p->z);

    bool is_even = (x_int + y_int + z_int) % 2 == 0;

    if (is_even) {
      return texture_value(checker->even, u, v, p);
    } else {
      return texture_value(checker->odd, u, v, p);
    }
  }
  case TEXTURE_IMGAGE: {
    image_texture_t* image = (image_texture_t*)texture;

    if (image->image.image_height <= 0) {
      return col_create(0, 1, 1);
    }

    interval_t clamp_interval = interval_create(0, 1);
    u = interval_clamp(&clamp_interval, u);
    v = interval_clamp(&clamp_interval, v);

    int i = (int)(u * image->image.image_width);
    int j = (int)(v * image->image.image_height);
    const unsigned char* pixel = image_pixel_data(&image->image, i, j);

    double color_scale = 1.0 / 255.0;
    return col_create(color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2]);
  }
  default:
    return col_create(0, 0, 0);
  }
}

solid_texture_t solid_texture_create(color_t color) {
  solid_texture_t result = {.base = {TEXTURE_SOLID}, .albedo = color};
  return result;
}

solid_texture_t solid_texture_create_rgb(double red, double green, double blue) {
  solid_texture_t result = {.base = {TEXTURE_SOLID}, .albedo = col_create(red, green, blue)};
  return result;
}

checker_texture_t checker_texture_create(double scale, texture_t* even, texture_t* odd) {
  checker_texture_t result = {.base = {TEXTURE_CHECKER}, .inv_scale = 1.0 / scale, .even = even, .odd = odd};
  return result;
}

checker_texture_t checker_texture_create_from_colors(double scale, color_t c1, color_t c2) {
  solid_texture_t* color1 = malloc(sizeof(solid_texture_t));
  solid_texture_t* color2 = malloc(sizeof(solid_texture_t));

  *color1 = solid_texture_create(c1);
  *color2 = solid_texture_create(c2);

  return checker_texture_create(scale, (texture_t*)color1, (texture_t*)color2);
}

image_texture_t image_texture_create(const char* filename) {
  image_texture_t result = {.base = {TEXTURE_IMGAGE}, .image = image_create(filename)};
  return result;
}
