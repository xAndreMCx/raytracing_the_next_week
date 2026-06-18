#include "scene.h"

#include <stdio.h>
#include <stdlib.h>

#include "hittable_list.h"
#include "material.h"
#include "sphere.h"
#include "texture.h"
#include "utils.h"
#include "vec.h"

static scene_registry_t scenes[] = {{0, "bouncing_spheres", scene_bouncing_spheres},
                                    {1, "checker_spheres", scene_checker_spheres},
                                    {2, "earth", scene_earth},
};

void scene_bouncing_spheres(hittable_list_t* world, camera_t* camera) {
  // Camera
  double aspect_ratio = 16.0 / 9.0;
  unsigned int width = 1280u;
  *camera = camera_create(width, aspect_ratio, vec3_create(13, 2, 3), vec3_create(0, 0, 0), vec3_create(0, 1, 0), 20.0);

  camera->max_depth = 50;
  camera->samples_per_pixel = 500;

  // World
  checker_texture_t* checker = malloc(sizeof(checker_texture_t));
  *checker = checker_texture_create_from_colors(0.32, col_create(0.2, 0.3, 0.1), col_create(0.9, 0.9, 0.9));

  lambertian_t* material_ground = malloc(sizeof(lambertian_t));
  *material_ground = lambertian_create(&checker->base);

  sphere_t* sphere_ground = malloc(sizeof(sphere_t));
  *sphere_ground = sphere_create(vec3_create(0, -1000, 0), 1000, &material_ground->base);
  hittable_list_add(world, &sphere_ground->base);

  for (int x = -11; x < 11; x++) {
    for (int y = -11; y < 11; y++) {
      double choose_mat = rand_double(0, 1);
      vec3_t center = vec3_create(x + 0.9 * rand_double(0, 1), 0.2, y + 0.9 * rand_double(0, 1));

      if (vec3_length(vec3_sub(center, vec3_create(4, 0.2, 0))) > 0.9) {
        if (choose_mat < 0.8) {
          // Diffuse
          vec3_t albedo = vec3_hadamard(vec3_create_random(0, 1), vec3_create_random(0, 1));
          lambertian_t* material = malloc(sizeof(lambertian_t));
          *material = lambertian_create_from_color(albedo);
          vec3_t center2 = vec3_add(center, vec3_create(0, rand_double(0, 0.5), 0));
          sphere_t* sphere = malloc(sizeof(sphere_t));
          *sphere = sphere_moving_create(center, center2, 0.2, &material->base);
          hittable_list_add(world, &sphere->base);
        } else if (choose_mat < 0.95) {
          // Metal
          vec3_t albedo = vec3_create_random(0.5, 1);
          double fuzz = rand_double(0, 0.5);
          metal_t* material = malloc(sizeof(metal_t));
          *material = metal_create(albedo, fuzz);
          sphere_t* sphere = malloc(sizeof(sphere_t));
          *sphere = sphere_create(center, 0.2, &material->base);
          hittable_list_add(world, &sphere->base);
        } else {
          // Glass
          dielectric_t* material = malloc(sizeof(dielectric_t));
          *material = dielectric_create(1.5);
          sphere_t* sphere = malloc(sizeof(sphere_t));
          *sphere = sphere_create(center, 0.2, &material->base);
          hittable_list_add(world, &sphere->base);
        }
      }
    }
  }

  dielectric_t* material1 = malloc(sizeof(dielectric_t));
  *material1 = dielectric_create(1.5);
  sphere_t* sphere1 = malloc(sizeof(sphere_t));
  *sphere1 = sphere_create(vec3_create(0, 1, 0), 1.0, &material1->base);
  hittable_list_add(world, &sphere1->base);

  lambertian_t* material2 = malloc(sizeof(lambertian_t));
  *material2 = lambertian_create_from_color(col_create(0.4, 0.2, 0.1));
  sphere_t* sphere2 = malloc(sizeof(sphere_t));
  *sphere2 = sphere_create(vec3_create(-4, 1, 0), 1.0, &material2->base);
  hittable_list_add(world, &sphere2->base);

  metal_t* material3 = malloc(sizeof(metal_t));
  *material3 = metal_create(col_create(0.7, 0.6, 0.5), 0.0);
  sphere_t* sphere3 = malloc(sizeof(sphere_t));
  *sphere3 = sphere_create(vec3_create(4, 1, 0), 1.0, &material3->base);
  hittable_list_add(world, &sphere3->base);

  return;
}

void scene_checker_spheres(hittable_list_t* world, camera_t* camera) {
  // Camera
  double aspect_ratio = 16.0 / 9.0;
  unsigned int width = 400u;
  *camera = camera_create(width, aspect_ratio, vec3_create(13, 2, 3), vec3_create(0, 0, 0), vec3_create(0, 1, 0), 20.0);

  camera->max_depth = 50;
  camera->samples_per_pixel = 100;
  camera->defocus_angle = 0.0f;

  // World
  checker_texture_t* checker = malloc(sizeof(checker_texture_t));
  *checker = checker_texture_create_from_colors(0.32, col_create(0.2, 0.3, 0.1), col_create(0.9, 0.9, 0.9));

  lambertian_t* material_ground = malloc(sizeof(lambertian_t));
  *material_ground = lambertian_create(&checker->base);

  sphere_t* sphere_bottom = malloc(sizeof(sphere_t));
  *sphere_bottom = sphere_create(vec3_create(0, -10, 0), 10, &material_ground->base);
  hittable_list_add(world, &sphere_bottom->base);

  sphere_t* sphere_top = malloc(sizeof(sphere_t));
  *sphere_top = sphere_create(vec3_create(0, 10, 0), 10, &material_ground->base);
  hittable_list_add(world, &sphere_top->base);

  return;
}

void scene_earth(hittable_list_t* world, camera_t* camera) {
  // Camera
  double aspect_ratio = 16.0 / 9.0;
  unsigned int width = 1280u;
  *camera = camera_create(width, aspect_ratio, vec3_create(12, 2, -3), vec3_create(0, 0, 0), vec3_create(0, 1, 0), 20.0);

  camera->max_depth = 50;
  camera->samples_per_pixel = 200;
  camera->defocus_angle = 0.0f;

  // World
  image_texture_t* texture_earth = malloc(sizeof(image_texture_t));
  *texture_earth = image_texture_create("earthmap.jpg");

  lambertian_t* material_earth = malloc(sizeof(lambertian_t));
  *material_earth = lambertian_create(&texture_earth->base);

  sphere_t* sphere_earth = malloc(sizeof(sphere_t));
  *sphere_earth = sphere_create(vec3_create(0, 0, 0), 2, &material_earth->base);
  hittable_list_add(world, &sphere_earth->base);
  
  return;
}

scene_builder_fn scene_get_builder(int id) {
  int num_scenes = sizeof(scenes) / sizeof(scenes[0]);

  for (int i = 0; i < num_scenes; i++) {
    if (scenes[i].id == id) {
      printf("Loading Scene %d: %s\n", scenes[i].id, scenes[i].name);
      return scenes[i].build;
    }
  }

  // Default fallback
  printf("Unknown scene ID %d. Defaulting to Scene 1: %s\n", id, scenes[0].name);
  return scenes[0].build;
}
