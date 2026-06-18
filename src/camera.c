#include "camera.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "material.h"
#include "ppm.h"
#include "utils.h"

camera_t camera_create(unsigned int image_width, double aspect_ratio, vec3_t look_from, vec3_t look_at, vec3_t up,
                       double field_of_view) {
  camera_t result;
  result.samples_per_pixel = 500u;
  result.max_depth = 100u;
  result.defocus_angle = 0.6;
  result.focus_distance = 10;

  result.image_width = image_width;
  result.aspect_ratio = aspect_ratio;
  result.center = look_from;
  result.field_of_view = field_of_view;

  int img_height = image_width / aspect_ratio;
  result.image_height = (img_height < 1) ? 1 : img_height;

  double theta = DEG_TO_RAD(result.field_of_view);
  double h = tan(theta / 2);
  double viewport_height = 2.0 * h * result.focus_distance;
  double viewport_width = viewport_height * ((double)image_width / result.image_height);

  result.w = vec3_normalised(vec3_sub(look_from, look_at));
  result.u = vec3_normalised(vec3_cross(up, result.w));
  result.v = vec3_cross(result.w, result.u);

  vec3_t viewport_u = vec3_scale(result.u, viewport_width);
  vec3_t viewport_v = vec3_scale(result.v, -viewport_height);

  result.pixel_delta_u = vec3_div(viewport_u, result.image_width);
  result.pixel_delta_v = vec3_div(viewport_v, result.image_height);

  vec3_t viewport_upper_left =
      vec3_sub(vec3_sub(vec3_sub(result.center, vec3_scale(result.w, result.focus_distance)), vec3_div(viewport_u, 2)),
               vec3_div(viewport_v, 2));
  result.pixel_upper_left =
      vec3_add(viewport_upper_left, vec3_scale(vec3_add(result.pixel_delta_u, result.pixel_delta_v), 0.5));

  double defocus_radius = result.focus_distance * tan(DEG_TO_RAD(result.defocus_angle / 2));
  result.defocus_disk_u = vec3_scale(result.u, defocus_radius);
  result.defocus_disk_v = vec3_scale(result.v, defocus_radius);

  return result;
}

void* render_pixel_worker(void* arg) {
  thread_data_t* data = (thread_data_t*)arg;
  unsigned int width = data->camera->image_width;

  unsigned int my_seed = (unsigned int)time(NULL) ^ (unsigned int)(uintptr_t)pthread_self();
  seed_thread_rng(my_seed);

  seed_thread_rng((unsigned int)(uintptr_t)pthread_self());

  while (1) {
    unsigned int index = atomic_fetch_add(data->next_pixel_index, 1);

    if (index >= data->total_pixels)
      break;

    unsigned int x = index % width;
    unsigned int y = index / width;

    color_t pixel_color = col_create(0, 0, 0);
    for (unsigned int sample = 0; sample < data->camera->samples_per_pixel; sample++) {
      ray_t ray = ray_get(data->camera, x, y);
      pixel_color = vec3_add(pixel_color, ray_color(&ray, data->camera->max_depth, data->world));
    }

    pixel_color = vec3_scale(pixel_color, 1.0f / data->camera->samples_per_pixel);

    // Gamma correction
    pixel_color = vec3_map(pixel_color, linear_to_gamma);

    interval_t intensity = interval_create(0.0, 1.0);
    pixel_color.r = interval_clamp(&intensity, pixel_color.r);
    pixel_color.g = interval_clamp(&intensity, pixel_color.g);
    pixel_color.b = interval_clamp(&intensity, pixel_color.b);

    ppm_set(data->render_result, x, y, pixel_color);
  }
  return NULL;
}

void render(camera_t* camera, hittable_list_t* world, const char* filepath) {
  PPM render_result = ppm_create(camera->image_width, camera->image_height);
  atomic_uint next_pixel_index = 0;
  unsigned int total_pixels = camera->image_width * camera->image_height;

  thread_data_t* pool_data = malloc(sizeof(thread_data_t));
  pool_data->camera = camera;
  pool_data->world = world;
  pool_data->render_result = &render_result;
  pool_data->next_pixel_index = &next_pixel_index;
  pool_data->total_pixels = total_pixels;

  const int num_threads = 8;
  pthread_t threads[num_threads];

  for (int i = 0; i < num_threads; i++) {
    int result = pthread_create(&threads[i], NULL, render_pixel_worker, pool_data);

    if (result != 0) {
      fprintf(stderr, "Error: pthread_create failed with error code %d\n", result);

      for (int j = 0; j < i; j++) {
        pthread_join(threads[j], NULL);
      }
      free(pool_data);
      exit(1);
    }
  }

  for (int i = 0; i < num_threads; i++) {
    pthread_join(threads[i], NULL);
  }

  ppm_write(&render_result, filepath);
  ppm_free(&render_result);
  free(pool_data);
}

color_t ray_color(ray_t* ray, unsigned int depth, hittable_list_t* world) {
  assert(ray);
  assert(world);

  if (depth <= 0) {
    return col_create(0, 0, 0);
  }

  hit_record_t hit_record;
  if (hittable_list_hit(world, ray, &(interval_t){.min = 0.0001, .max = INFINITY}, &hit_record)) {
    ray_t scattered;
    color_t attenuation;
    if (material_scatter((material_t*)hit_record.material, ray, &hit_record, &attenuation, &scattered)) {
      return vec3_hadamard(attenuation, ray_color(&scattered, depth - 1, world));
    }
    return col_create(0, 0, 0);
  }

  // Background
  vec3_t unit_vec = vec3_normalised(ray->direction);
  double a = (unit_vec.y + 1.00) * 0.5;
  return vec3_add(vec3_scale(col_create(1.0, 1.0, 1.0), 1.00 - a), vec3_scale(col_create(0.5, 0.7, 1.0), a));
}

ray_t ray_get(camera_t* camera, unsigned int x, unsigned int y) {
  vec3_t pixel_center = vec3_add(camera->pixel_upper_left,
                                 vec3_add(vec3_scale(camera->pixel_delta_u, x), vec3_scale(camera->pixel_delta_v, y)));

  double px = -0.5 + rand_double(0.0, 1.0);
  double py = -0.5 + rand_double(0.0, 1.0);
  vec3_t rand_pixel = vec3_add(vec3_scale(camera->pixel_delta_u, px), vec3_scale(camera->pixel_delta_v, py));
  vec3_t pixel_sample = vec3_add(pixel_center, rand_pixel);

  vec3_t ray_origin = (camera->defocus_angle <= 0) ? camera->center : camera_defocus_disk_sample(camera);
  vec3_t ray_direction = vec3_sub(pixel_sample, ray_origin);
  double ray_time = rand_double(0, 1);

  ray_t ray = {ray_origin, ray_direction, ray_time};

  return ray;
}

vec3_t camera_defocus_disk_sample(camera_t* camera) {
  vec3_t p = vec3_random_in_unit_disk();
  vec3_t result = vec3_add(camera->center,
                           vec3_add(vec3_scale(camera->defocus_disk_u, p.x), vec3_scale(camera->defocus_disk_v, p.y)));
  return result;
}
