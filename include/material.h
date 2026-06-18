#pragma once
#include <stdbool.h>

#include "hittable.h"
#include "ray.h"
#include "texture.h"
#include "vec.h"

/**
 * @enum material_type_t
 * @brief An enumeration of different types of materials.
 */
typedef enum { MATERIAL_UNKOWN = -1, MATERIAL_LAMBERTIAN = 0, MATERIAL_METAL = 1, MATERIAL_DIELECTRIC = 2 } material_type_t;

/**
 * @struct material_t
 * @brief A structure to represent a material.
 */
typedef struct {
  material_type_t type;
} material_t;

/**
 * @brief Scatters a ray based on the material properties.
 *
 * @param material Pointer to the material.
 * @param ray Pointer to the incoming ray.
 * @param hit_record Pointer to the hit record.
 * @param attenuation Pointer to the color attenuation.
 * @param scattered_ray Pointer to the scattered ray.
 * @return True if the ray is scattered, false otherwise.
 */
bool material_scatter(material_t* material, ray_t* ray, hit_record_t* hit_record, color_t* attenuation, ray_t* scattered_ray);

/**
 * @struct lambertian_t
 * @brief A structure to represent a Lambertian material.
 */
typedef struct {
  material_t base;
  texture_t* texture;
} lambertian_t;

/**
 * @brief Creates a Lambertian material with the specified albedo.
 *
 * @param albedo The albedo color of the material.
 * @return A Lambertian material with the specified albedo.
 */
lambertian_t lambertian_create_from_color(color_t albedo);

/**
 * @brief Creates a Lambertian material with the specified texture.
 *
 * @param texture The texture of the material.
 * @return A Lambertian material with the specified texture.
 */
lambertian_t lambertian_create(texture_t* texture);

/**
 * @brief Scatters a ray based on Lambertian material properties.
 *
 * @param material Pointer to the Lambertian material.
 * @param ray Pointer to the incoming ray.
 * @param hit_record Pointer to the hit record.
 * @param attenuation Pointer to the color attenuation.
 * @param scattered_ray Pointer to the scattered ray.
 * @return True if the ray is scattered, false otherwise.
 */
bool lambertian_scatter(lambertian_t* material, ray_t* ray, hit_record_t* hit_record, color_t* attenuation, ray_t* scattered_ray);

/**
 * @struct metal_t
 * @brief A structure to represent a Metal material.
 */
typedef struct {
  material_t base;
  color_t albedo; // Albedo color of the material.
  double fuzz;    // Fuzziness factor of the material.
} metal_t;

/**
 * @brief Creates a Metal material with the specified albedo and fuzziness.
 *
 * @param albedo The albedo color of the material.
 * @param fuzz The fuzziness factor of the material.
 * @return A Metal material with the specified albedo and fuzziness.
 */
metal_t metal_create(color_t albedo, double fuzz);

/**
 * @brief Scatters a ray based on Metal material properties.
 *
 * @param material Pointer to the Metal material.
 * @param ray Pointer to the incoming ray.
 * @param hit_record Pointer to the hit record.
 * @param attenuation Pointer to the color attenuation.
 * @param scattered_ray Pointer to the scattered ray.
 * @return True if the ray is scattered, false otherwise.
 */
bool metal_scatter(metal_t* material, ray_t* ray, hit_record_t* hit_record, color_t* attenuation, ray_t* scattered_ray);

/**
 * @struct dielectric_t
 * @brief A structure to represent a Dielectric material.
 */
typedef struct {
  material_t base;
  double refraction_index;
} dielectric_t;

/**
 * @brief Creates a Dielectric material with the specified refraction index.
 *
 * @param refraction_index The refraction index of the material.
 * @return A Dielectric material with the specified refraction index.
 */
dielectric_t dielectric_create(double refraction_index);

/**
 * @brief Scatters a ray based on Dielectric material properties.
 *
 * @param material Pointer to the Dielectric material.
 * @param ray Pointer to the incoming ray.
 * @param hit_record Pointer to the hit record.
 * @param attenuation Pointer to the color attenuation.
 * @param scattered_ray Pointer to the scattered ray.
 * @return True if the ray is scattered, false otherwise.
 */
bool dielectric_scatter(dielectric_t* material, ray_t* ray, hit_record_t* hit_record, color_t* attenuation, ray_t* scattered_ray);
