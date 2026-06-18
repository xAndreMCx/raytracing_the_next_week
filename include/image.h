#pragma once

#include <stdbool.h>

/**
 * @struct image_t
 * @brief A structure to manage loading and accessing image pixel data.
 */
typedef struct {
  float* fdata;           // Linear floating point pixel data
  unsigned char* bdata;   // Linear 8-bit pixel data
  int image_width;        // Loaded image width
  int image_height;       // Loaded image height
  int bytes_per_scanline; // Number of bytes per row
} image_t;

/**
 * @brief Loads an image from a file, checking multiple fallback directories.
 *
 * @param image_filename The name of the image file to load.
 * @return An initialized image structure. Width and height are 0 if loading fails.
 */
image_t image_create(const char* image_filename);

/**
 * @brief Frees the memory allocated by the image.
 *
 * @param img Pointer to the image to free.
 */
void image_free(image_t* img);

/**
 * @brief Gets the RGB pixel data at a specific coordinate.
 *
 * @param img Pointer to the image.
 * @param x The x coordinate.
 * @param y The y coordinate.
 * @return A pointer to the 3 bytes (RGB) of the requested pixel. Returns magenta if no image data exists.
 */
const unsigned char* image_pixel_data(const image_t* img, int x, int y);
