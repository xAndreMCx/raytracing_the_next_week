#pragma once
#include <stdatomic.h>

#include "hittable_list.h"
#include "ppm.h"
#include "ray.h"
#include "vec.h"

/**
 * @struct camera_t
 * @brief A structure to represent a camera in the scene.
 */
typedef struct {
  unsigned int image_width;
  unsigned int image_height;
  double aspect_ratio;
  vec3_t center;
  vec3_t pixel_upper_left;
  vec3_t pixel_delta_u;
  vec3_t pixel_delta_v;
  unsigned int samples_per_pixel;
  unsigned int max_depth;
  double field_of_view;
  vec3_t u;
  vec3_t v;
  vec3_t w;
  vec3_t defocus_disk_u;
  vec3_t defocus_disk_v;
  double defocus_angle;
  double focus_distance;
  color_t background;
} camera_t;

/**
 * @struct thread_data_t
 * @brief Payload structure passed to each pthread.
 */
typedef struct {
  camera_t* camera;
  hittable_list_t* world;
  PPM* render_result;
  atomic_uint* next_pixel_index;
  unsigned int total_pixels;
} thread_data_t;

/**
 * @brief Creates a camera with the specified parameters.
 *
 * @param image_width Width of the image.
 * @param aspect_ratio Aspect ratio of the image.
 * @param look_from Position of the camera.
 * @param look_at Point the camera is looking at.
 * @param up Up direction vector.
 * @param field_of_view Field of view of the camera.
 * @return A camera with the specified parameters.
 */
camera_t camera_create(unsigned int image_width, double aspect_ratio, vec3_t look_from, vec3_t look_at, vec3_t up,
                       double field_of_view);

/**
 * @brief The worker function executed by each pthread.
 *
 * @param arg Pointer to thread_data_t.
 * @return void* Required by pthread_create.
 */
void* render_pixel_worker(void* arg);

/**
 * @brief Renders the scene and saves the image to a file.
 *
 * @param camera Pointer to the camera.
 * @param world Pointer to the hittable list representing the world.
 * @param filepath Path to the file where the image will be saved.
 */
void render(camera_t* camera, hittable_list_t* world, const char* filepath);

/**
 * @brief Computes the color of a ray.
 *
 * @param camera Pointer to the camera.
 * @param ray Pointer to the ray.
 * @param depth Current depth of the ray.
 * @param world Pointer to the hittable list representing the world.
 * @return The color of the ray.
 */
color_t ray_color(camera_t* camera, ray_t* ray, unsigned int depth, hittable_list_t* world);

/**
 * @brief Generates a ray from the camera through a specific pixel.
 *
 * @param camera Pointer to the camera.
 * @param x X-coordinate of the pixel.
 * @param y Y-coordinate of the pixel.
 * @return A ray from the camera through the specified pixel.
 */
ray_t ray_get(camera_t* camera, unsigned int x, unsigned int y);

/**
 * @brief Samples a point on the camera's defocus disk.
 *
 * @param camera Pointer to the camera.
 * @return A point on the camera's defocus disk.
 */
vec3_t camera_defocus_disk_sample(camera_t* camera);
