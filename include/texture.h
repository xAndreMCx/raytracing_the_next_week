#pragma once

#include "vec.h"
#include "image.h"

/**
 * @enum texture_type_t
 * @brief An enumeration of different types of textures.
 */
typedef enum { TEXTURE_SOLID, TEXTURE_CHECKER, TEXTURE_IMGAGE } texture_type_t;

/**
 * @struct texture_t
 * @brief Base structure for all textures to enable polymorphism.
 */
typedef struct {
  texture_type_t type;
} texture_t;

/**
 * @struct solid_texture_t
 * @brief A texture that consists of a single, uniform color.
 */
typedef struct {
  texture_t base;
  color_t albedo;
} solid_texture_t;

/**
 * @struct checker_texture_t
 * @brief A procedural texture that alternates between two other textures.
 */
typedef struct {
  texture_t base;
  double inv_scale;
  texture_t* even;
  texture_t* odd;
} checker_texture_t;

/**
 * @struct image_texture_t
 * @brief A image texture.
 */
typedef struct {
  texture_t base;
  image_t image;
} image_texture_t ;

/**
 * @brief Gets the color value of a texture at specific coordinates.
 *
 * @param texture Pointer to the texture.
 * @param u The U surface coordinate.
 * @param v The V surface coordinate.
 * @param p Pointer to the 3D point of intersection.
 * @return The evaluated color.
 */
color_t texture_value(texture_t* texture, double u, double v, vec3_t* p);

/**
 * @brief Creates a solid color texture.
 *
 * @param color The uniform color.
 * @return A solid color texture.
 */
solid_texture_t solid_texture_create(color_t color);

/**
 * @brief Creates a solid color texture from rgb values.
 *
 * @param red The red color compnenet.
 * @param green The green color compnenet.
 * @param blue The blue color compnenet.
 * @return A solid color texture.
 */
solid_texture_t solid_texture_create_rgb(double red, double green, double blue);

/**
 * @brief Creates a checkerboard texture from two other textures.
 *
 * @param scale The scaling factor for the checker pattern.
 * @param even Pointer to the even texture.
 * @param odd Pointer to the odd texture.
 * @return A checker texture.
 */
checker_texture_t checker_texture_create(double scale, texture_t* even, texture_t* odd);

/**
 * @brief Creates a checkerboard texture from two colors.
 *
 * @param scale The scaling factor for the checker pattern.
 * @param c1 The first color.
 * @param c2 The second color.
 * @return A checker texture.
 */
checker_texture_t checker_texture_create_from_colors(double scale, color_t c1, color_t c2);

/**
 * @brief Creates a texture from an image.
 *
 * @param filename The filename of the image.
 * @return An image texture.
 */
image_texture_t image_texture_create(const char* filename);
