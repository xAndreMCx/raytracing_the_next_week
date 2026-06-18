#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "bvh.h"
#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"
#include "utils.h"
#include "vec.h"

int main(void) {
  seed_thread_rng((unsigned int)time(NULL));

  // Camera
  double aspect_ratio = 16.0 / 9.0;
  unsigned int width = 400u;
  camera_t camera =
      camera_create(width, aspect_ratio, vec3_create(13, 2, 3), vec3_create(0, 0, 0), vec3_create(0, 1, 0), 20.0);

  camera.max_depth = 50;
  camera.samples_per_pixel = 100;

  // World
  hittable_list_t world = hittable_list_create(400);

  lambertian_t material_ground = lambertian_create(col_create(0.5, 0.5, 0.5));
  sphere_t sphere_ground = sphere_create(vec3_create(0, -1000, 0), 1000, &material_ground.base);
  hittable_list_add(&world, &sphere_ground.base);

  for (int x = -11; x < 11; x++) {
    for (int y = -11; y < 11; y++) {
      double choose_mat = rand_double(0, 1);
      vec3_t center = vec3_create(x + 0.9 * rand_double(0, 1), 0.2, y + 0.9 * rand_double(0, 1));

      if (vec3_length(vec3_sub(center, vec3_create(4, 0.2, 0))) > 0.9) {
        if (choose_mat < 0.8) {
          // Diffuse
          vec3_t albedo = vec3_hadamard(vec3_create_random(0, 1), vec3_create_random(0, 1));
          lambertian_t* material = malloc(sizeof(lambertian_t));
          *material = lambertian_create(albedo);
          vec3_t center2 = vec3_add(center, vec3_create(0, rand_double(0, 0.5), 0));
          sphere_t* sphere = malloc(sizeof(sphere_t));
          *sphere = sphere_moving_create(center, center2, 0.2, &material->base);
          hittable_list_add(&world, &sphere->base);
        } else if (choose_mat < 0.95) {
          // Metal
          vec3_t albedo = vec3_create_random(0.5, 1);
          double fuzz = rand_double(0, 0.5);
          metal_t* material = malloc(sizeof(metal_t));
          *material = metal_create(albedo, fuzz);
          sphere_t* sphere = malloc(sizeof(sphere_t));
          *sphere = sphere_create(center, 0.2, &material->base);
          hittable_list_add(&world, &sphere->base);
        } else {
          // Glass
          dielectric_t material = dielectric_create(1.5);
          sphere_t* sphere = malloc(sizeof(sphere_t));
          *sphere = sphere_create(center, 0.2, &material.base);
          hittable_list_add(&world, &sphere->base);
        }
      }
    }
  }

  dielectric_t material1 = dielectric_create(1.5);
  sphere_t sphere1 = sphere_create(vec3_create(0, 1, 0), 1.0, &material1.base);
  hittable_list_add(&world, &sphere1.base);

  lambertian_t material2 = lambertian_create(col_create(0.4, 0.2, 0.1));
  sphere_t sphere2 = sphere_create(vec3_create(-4, 1, 0), 1.0, &material2.base);
  hittable_list_add(&world, &sphere2.base);

  metal_t material3 = metal_create(col_create(0.7, 0.6, 0.5), 0.0);
  sphere_t sphere3 = sphere_create(vec3_create(4, 1, 0), 1.0, &material3.base);
  hittable_list_add(&world, &sphere3.base);

  printf("Building BVH tree.\n");
  bvh_node_t* bvh_root = bvh_node_create(&world, 0, world.length);

  hittable_list_t bvh_world = hittable_list_create(1);
  hittable_list_add(&bvh_world, (hittable_t*)bvh_root);

  printf("Starting render.\n");
  render(&camera, &bvh_world, "./test.ppm");

  hittable_list_free(&world);
  // TODO: free bvh here

  printf("Done.\n");

  return 0;
}
