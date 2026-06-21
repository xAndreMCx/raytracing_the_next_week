project(
  'raytracer',
  'c',
  version: '0.1.0',
  default_options: ['c_std=c11', 'warning_level=3'],
)

inc = include_directories('include', 'vendor/stb')

cc = meson.get_compiler('c')
m_dep = cc.find_library('m', required: true)
thread_dep = dependency('threads')

src_files = [
  'src/main.c',
  'src/camera.c',
  'src/hittable.c',
  'src/hittable_list.c',
  'src/interval.c',
  'src/material.c',
  'src/ppm.c',
  'src/ray.c',
  'src/sphere.c',
  'src/utils.c',
  'src/vec.c',
  'src/aabb.c',
  'src/bvh.c',
  'src/texture.c',
  'src/scene.c',
  'src/image.c',
  'src/perlin.c',

  'vendor/stb/stb_impl.c',
]

executable(
  'raytracer',
  src_files,
  include_directories: inc,
  dependencies: [m_dep, thread_dep],
  install: true,
)
