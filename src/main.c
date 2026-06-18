#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "bvh.h"
#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "utils.h"
#include "scene.h"

int main(int argc, char** argv) {
  seed_thread_rng((unsigned int)time(NULL));

  hittable_list_t world = hittable_list_create(400);
  camera_t camera;

  int scene_id = 0;
  if (argc > 1) {
    scene_id = atoi(argv[1]);
  }

  scene_builder_fn build_scene = scene_get_builder(scene_id);
  build_scene(&world, &camera);

  printf("Building BVH tree.\n");
  bvh_node_t* bvh_root = bvh_node_create(&world, 0, world.length);

  hittable_list_t bvh_world = hittable_list_create(1);
  hittable_list_add(&bvh_world, (hittable_t*)bvh_root);

  printf("Starting render.\n");
  render(&camera, &bvh_world, "./test.ppm");

  hittable_list_free(&bvh_world);
  hittable_list_free(&world);

  printf("Done.\n");

  return 0;
}
