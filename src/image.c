#include "image.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stb_image.h"

#define BYTES_PER_PIXEL 3

static int clamp(int x, int low, int high) {
  if (x < low)
    return low;
  if (x < high)
    return x;
  return high - 1;
}

static unsigned char float_to_byte(float value) {
  if (value <= 0.0f)
    return 0;
  if (1.0f <= value)
    return 255;
  return (unsigned char)(256.0f * value);
}

static bool load_internal(image_t* img, const char* filepath) {
  int n; // Dummy out parameter
  img->fdata = stbi_loadf(filepath, &img->image_width, &img->image_height, &n, BYTES_PER_PIXEL);
  if (!img->fdata) {
    return false;
  }

  img->bytes_per_scanline = img->image_width * BYTES_PER_PIXEL;
  int total_bytes = img->image_width * img->image_height * BYTES_PER_PIXEL;
  img->bdata = malloc(total_bytes);

  // Convert the linear floating point pixel data to bytes
  for (int i = 0; i < total_bytes; i++) {
    img->bdata[i] = float_to_byte(img->fdata[i]);
  }

  return true;
}

image_t image_create(const char* image_filename) {
  image_t img = {0};

  char path_buffer[1024];

  const char* search_paths[] = {"",
                                "images/",
                                "../images/",
                                "../../images/",
                                "../../../images/",
                                "../../../../images/",
                                "../../../../../images/",
                                "../../../../../../images/"};

  int num_paths = sizeof(search_paths) / sizeof(search_paths[0]);
  for (int i = 0; i < num_paths; i++) {
    snprintf(path_buffer, sizeof(path_buffer), "%s%s", search_paths[i], image_filename);
    if (load_internal(&img, path_buffer)) {
      return img;
    }
  }

  fprintf(stderr, "ERROR: Could not load image file '%s'.\n", image_filename);
  return img;
}

void image_free(image_t* img) {
  assert(img);

  if (img->bdata) {
    free(img->bdata);
    img->bdata = NULL;
  }

  if (img->fdata) {
    stbi_image_free(img->fdata);
    img->fdata = NULL;
  }

  img->image_width = 0;
  img->image_height = 0;
  img->bytes_per_scanline = 0;
}

const unsigned char* image_pixel_data(const image_t* img, int x, int y) {
  static const unsigned char magenta[] = {255, 0, 255};

  if (!img->bdata) {
    return magenta;
  }

  x = clamp(x, 0, img->image_width);
  y = clamp(y, 0, img->image_height);

  return img->bdata + (y * img->bytes_per_scanline) + (x * BYTES_PER_PIXEL);
}
