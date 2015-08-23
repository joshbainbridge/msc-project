#include <map>
#include <vector>
#include <iostream>

#include <core/Integrator.h>
#include <core/ObjectInterface.h>
#include <core/ShaderInterface.h>
#include <core/LightInterface.h>

MSC_NAMESPACE_BEGIN

void Integrator::operator()(const RangeGeom< RayUncompressed* > &r) const
{
  LocalRandomGenerator::reference random = m_local_thread_storage_random->local();
  LocalTextureSystem::reference texture_system = m_local_thread_storage_texture->local();
  
  if(texture_system == NULL)
    texture_system = OpenImageIO::TextureSystem::create(true);

  size_t range_size = (r.end() - r.begin());
  size_t geom_id = m_batch[r.begin()].geomID;
  size_t light_count = m_scene->lights.size();
  float light_pick_probability = 1.f / light_count;

  // If nothing was hit
  if(geom_id == -1)
    return;

  ObjectInterface* object = m_scene->objects[geom_id].get();

  int intersected_light = -1;
  std::map<int, int>::const_iterator iterator = m_scene->shaders_to_lights.find(object->shader());
  if(iterator != m_scene->shaders_to_lights.end())
    intersected_light = iterator->second;

  // If a light was hit
  if(intersected_light >= 0)
  {
    LightInterface* light = m_scene->lights[intersected_light].get();

    for(size_t index = r.begin(); index < r.end(); ++index)
    {
      Vector3f ray_direction = Vector3f(
        m_batch[index].dir[0],
        m_batch[index].dir[1],
        m_batch[index].dir[2]
        ).normalized();

      Colour3f light_radiance;
      float light_pdfa;
      float cos_theta;

      light->radiance(ray_direction, &light_radiance, &cos_theta, &light_pdfa);

      float mis_balance = 1.0f;
      if(m_batch[index].rayDepth > 0)
      {
        float light_pdfw = areaToAngleProbability(light_pdfa, m_batch[index].tfar, cos_theta);
        float last_pdfw = m_batch[index].lastPdf;
        mis_balance = misBalance(last_pdfw, light_pdfw * light_pick_probability);
      }
      // mis_balance = 0.5f;

      if(light_radiance.matrix().maxCoeff() > M_EPSILON)
      {
        m_image->samples[m_batch[index].sampleID].r += light_radiance[0]
         * mis_balance * m_batch[index].weight[0];
        m_image->samples[m_batch[index].sampleID].g += light_radiance[1]
         * mis_balance * m_batch[index].weight[1];
        m_image->samples[m_batch[index].sampleID].b += light_radiance[2]
         * mis_balance * m_batch[index].weight[2];
      }
    }

    return;
  }

  ShaderInterface* shader = m_scene->shaders[object->shader()]->clone();

  // Compute shader coefficients 
  {
    std::vector< float > u(range_size);
    std::vector< float > v(range_size);

    for(size_t index = 0; index < range_size; ++index)
    {
      Vector2f texture;
      object->texture(
        m_batch[r.begin() + index].primID,
        m_batch[r.begin() + index].u,
        m_batch[r.begin() + index].v,
        &texture
        );

      u[index] = texture[0];
      v[index] = texture[1];
    }

    shader->initialize(range_size, u, v, texture_system);
  }

  // Next event estimation
  {
    for(size_t index = r.begin(); index < r.end(); ++index)
    {
      size_t colour_index = index - r.begin();

      size_t ligt_identifier = size_t(light_count * random.sample());
      LightInterface* light = m_scene->lights[ligt_identifier].get();

      Vector3f ray_origin = Vector3f(
        m_batch[index].org[0],
        m_batch[index].org[1],
        m_batch[index].org[2]
        );
      Vector3f ray_direction = Vector3f(
        m_batch[index].dir[0],
        m_batch[index].dir[1],
        m_batch[index].dir[2]
        ).normalized();
      Vector3f normal = Vector3f(
        m_batch[index].Ng[0],
        m_batch[index].Ng[1],
        m_batch[index].Ng[2]
        ).normalized() * -1.f;
      Vector3f position = ray_origin + ray_direction * m_batch[index].tfar;
      Vector3f output_dir = ray_direction * -1.f;
      Vector3f input_dir;

      Colour3f light_radiance;
      float light_pdfw;
      float distance;

      light->illuminate(&random, position, &input_dir, &distance, &light_radiance, &light_pdfw);

      if(light_radiance.matrix().maxCoeff() > M_EPSILON)
      {
        Colour3f bsdf_weight;
        float bsdf_pdfw;
        float cos_theta;

        shader->evaluate(colour_index, input_dir, output_dir, normal, &bsdf_weight, &cos_theta, &bsdf_pdfw);

        if(bsdf_weight.matrix().maxCoeff() > M_EPSILON)
        {
          float mis_balance = misBalance(light_pdfw * light_pick_probability, bsdf_pdfw);
          // mis_balance = 0.5f;

          Colour3f contribution = (mis_balance
           * cos_theta / (light_pdfw * light_pick_probability))
           * (light_radiance * bsdf_weight);

          RayUncompressed shadow_ray;
          shadow_ray.tnear = 0.001f;
          shadow_ray.tfar = distance;
          shadow_ray.mask = 0x0FFFFFFF;
          shadow_ray.time = 0.f;
          shadow_ray.geomID = RTC_INVALID_GEOMETRY_ID;
          shadow_ray.primID = RTC_INVALID_GEOMETRY_ID;
          shadow_ray.instID = RTC_INVALID_GEOMETRY_ID;
          shadow_ray.org[0] = position[0];
          shadow_ray.org[1] = position[1];
          shadow_ray.org[2] = position[2];
          shadow_ray.dir[0] = input_dir[0];
          shadow_ray.dir[1] = input_dir[1];
          shadow_ray.dir[2] = input_dir[2];
          rtcOccluded(m_scene->rtc_scene, shadow_ray.rtc_ray);

          if(shadow_ray.geomID != 0)
          {
            m_image->samples[m_batch[index].sampleID].r += contribution[0] * m_batch[index].weight[0];
            m_image->samples[m_batch[index].sampleID].g += contribution[1] * m_batch[index].weight[1];
            m_image->samples[m_batch[index].sampleID].b += contribution[2] * m_batch[index].weight[2];
          }
        }
      }
    }
  }

  // Continue random walk
  {
    for(size_t index = r.begin(); index < r.end(); ++index)
    {
      if(m_batch[index].rayDepth >= m_settings->max_depth)
        continue;

      float tentative_contrib = shader->continuation();
      float cont_probability = fmin(1.f, tentative_contrib / m_settings->threshold);

      if(m_batch[index].rayDepth < m_settings->min_depth)
        cont_probability = 1.f;

      if(random.sample() > cont_probability)
        continue;

      size_t colour_index = index - r.begin();

      Vector3f ray_origin = Vector3f(
        m_batch[index].org[0],
        m_batch[index].org[1],
        m_batch[index].org[2]
        );
      Vector3f ray_direction = Vector3f(
        m_batch[index].dir[0],
        m_batch[index].dir[1],
        m_batch[index].dir[2]
        ).normalized();
      Vector3f normal = Vector3f(
        m_batch[index].Ng[0],
        m_batch[index].Ng[1],
        m_batch[index].Ng[2]
        ).normalized() * -1.f;
      Vector3f position = ray_origin + ray_direction * m_batch[index].tfar;
      Vector3f output_dir = ray_direction * -1.f;
      Vector3f input_dir;

      Colour3f bsdf_weight;
      float bsdf_pdfw;
      float cos_theta;

      shader->sample(&random, colour_index, output_dir, normal, &input_dir, &bsdf_weight, &cos_theta, &bsdf_pdfw);

      RayCompressed input_ray;
      input_ray.org[0] = position[0];
      input_ray.org[1] = position[1];
      input_ray.org[2] = position[2];
      input_ray.dir[0] = input_dir[0];
      input_ray.dir[1] = input_dir[1];
      input_ray.dir[2] = input_dir[2];
      input_ray.weight[0] = m_batch[index].weight[0]
       * bsdf_weight[0] * (cos_theta / bsdf_pdfw) / cont_probability;
      input_ray.weight[1] = m_batch[index].weight[1]
       * bsdf_weight[1] * (cos_theta / bsdf_pdfw) / cont_probability;
      input_ray.weight[2] = m_batch[index].weight[2]
       * bsdf_weight[2] * (cos_theta / bsdf_pdfw) / cont_probability;
      input_ray.lastPdf = bsdf_pdfw;
      input_ray.rayDepth = m_batch[index].rayDepth + 1;
      input_ray.sampleID = m_batch[index].sampleID;

      int max = (fabs(input_ray.dir[0]) < fabs(input_ray.dir[1])) ? 1 : 0;
      int axis = (fabs(input_ray.dir[max]) < fabs(input_ray.dir[2])) ? 2 : max;
      int cardinal = (input_ray.dir[axis] < 0.f) ? axis : axis + 3;

      m_buffer.direction[cardinal].push_back(input_ray);
    }
  }

  for(size_t index = 0; index < 6; ++index)
  {
    if(m_buffer.direction[index].size() > 0)
      m_bins->add(m_buffer.direction[index].size(), index, &(m_buffer.direction[index][0]), m_batch_queue);
  }

  delete shader;
}

MSC_NAMESPACE_END