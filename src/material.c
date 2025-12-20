#include "material.h"

#include "utils.h"

bool material_scatter(material_t* material, ray_t* ray, hit_record_t* hit_record, color_t* attenuation, ray_t* scattered_ray) {
  switch (material->type) {
    case MATERIAL_LAMBERTIAN:
      return lambertian_scatter((lambertian_t*)material, ray, hit_record, attenuation, scattered_ray);

    case MATERIAL_METAL:
      return metal_scatter((metal_t*)material, ray, hit_record, attenuation, scattered_ray);

    case MATERIAL_DIELECTRIC:
      return dielectric_scatter((dielectric_t*)material, ray, hit_record, attenuation, scattered_ray);

    default:
      return false;
  }
}

lambertian_t lambertian_create(color_t albedo) { return (lambertian_t){{MATERIAL_LAMBERTIAN}, albedo}; }

bool lambertian_scatter(lambertian_t* material, ray_t* ray, hit_record_t* hit_record, color_t* attenuation, ray_t* scattered_ray) {
  vec3_t scatter_direction = vec3_add(hit_record->normal, vec3_create_random_unit());
  if (vec3_near_zero(scatter_direction)) {
    scatter_direction = hit_record->normal;
  }
  *scattered_ray = (ray_t){hit_record->point, scatter_direction, ray->time};
  *attenuation = material->albedo;
  return true;
}

metal_t metal_create(color_t albedo, double fuzz) {
  fuzz = (fuzz < 1) ? fuzz : 1;
  metal_t result = {{MATERIAL_METAL}, albedo, fuzz};
  return result;
}

bool metal_scatter(metal_t* material, ray_t* ray, hit_record_t* hit_record, color_t* attenuation, ray_t* scattered_ray) {
  vec3_t reflected = vec3_reflect(vec3_normalised(ray->direction), hit_record->normal);
  reflected = vec3_add(reflected, vec3_scale(vec3_create_random_unit(), material->fuzz));
  *scattered_ray = (ray_t){hit_record->point, reflected, ray->time};
  *attenuation = material->albedo;
  return vec3_dot(scattered_ray->direction, hit_record->normal) > 0;
}

dielectric_t dielectric_create(double refraction_index) { return (dielectric_t){{MATERIAL_DIELECTRIC}, refraction_index}; }

static double refractance(double cosine, double refraction_index) {
  double r0 = (1 - refraction_index) / (1 + refraction_index);
  r0 = r0 * r0;
  return r0 + ((1 - r0) * pow(1 - cosine, 5));
}

bool dielectric_scatter(dielectric_t* material, ray_t* ray, hit_record_t* hit_record, color_t* attenuation, ray_t* scattered_ray) {
  *attenuation = col_create(1, 1, 1);
  double refraction_ratio = (hit_record->front_facing) ? (1.0 / material->refraction_index) : material->refraction_index;

  vec3_t unit_direction = vec3_normalised(ray->direction);
  double cos_theta = fmin(vec3_dot(vec3_negate(unit_direction), hit_record->normal), 1.0);
  double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

  bool cannot_refract = (refraction_ratio * sin_theta) > 1.0;
  vec3_t direction;

  if (cannot_refract || (refractance(cos_theta, refraction_ratio) > rand_double(0, 1))) {
    // Must relect
    direction = vec3_reflect(unit_direction, hit_record->normal);
  } else {
    // Can refract
    direction = vec3_refract(unit_direction, hit_record->normal, refraction_ratio);
  }

  *scattered_ray = (ray_t){hit_record->point, direction, ray->time};
  return true;
}
