#include <map>
#include <vector>
#include <iostream>

#include <core/Integrator.h>
#include <core/PolygonObject.h>
#include <core/ShaderInterface.h>
#include <core/LightInterface.h>

MSC_NAMESPACE_BEGIN

void Integrator::operator()(const RangeGeom< RayUncompressed* > &r) const
{
  LocalRandomGenerator::reference random = m_local_thread_storage_random->local();
  LocalTextureSystem::reference texture_system = m_local_thread_storage_texture->local();
  
  if(texture_system == NULL)
    texture_system = OpenImageIO::TextureSystem::create(true);

  // Get geometry id for group
  size_t geom_id = m_batch[r.begin()].geomID;

  // Did the points hit an object
  if(geom_id != -1)
  {
    // Which object did they hit
    PolygonObject* object = m_scene->objects[geom_id].get();

    // Check if object is a light
    int light_id = -1;

    std::map<int, int>::const_iterator iterator = m_scene->shaders_to_lights.find(object->shader);
    if(iterator != m_scene->shaders_to_lights.end())
      light_id = iterator->second;

    // If object is a surface
    if(light_id == -1)
    {
      ShaderInterface* shader = m_scene->shaders[object->shader].get();
      LightInterface* light = m_scene->lights[0].get();

      size_t range = r.end() - r.begin();

      float* result = new float[range * 3];
      float* u = new float[range];
      float* v = new float[range];
      Vector3f* input = new Vector3f[range];
      Vector3f* output = new Vector3f[range];
      Vector3f* normal = new Vector3f[range];
      Vector3f* position = new Vector3f[range];
      Colour3f* radiance = new Colour3f[range];

      //Pre loop through the group
      for(size_t index = r.begin(); index < r.end(); ++index)
      {
        size_t stack_index = index - r.begin();

        // Find the index to each vertex of the intersected primitive
        size_t vertex01 = object->indices[3 * m_batch[index].primID + 0];
        size_t vertex02 = object->indices[3 * m_batch[index].primID + 1];
        size_t vertex03 = object->indices[3 * m_batch[index].primID + 2];

        // The primitives barycentric coordinates
        float s = m_batch[index].u;
        float t = m_batch[index].v;

        // The intersected texture coordinates
        u[stack_index] = (1.f - s - t) * object->texcoords[2 * vertex01 + 0] + 
        s * object->texcoords[2 * vertex02 + 0] + 
        t * object->texcoords[2 * vertex03 + 0];

        v[stack_index] = (1.f - s - t) * object->texcoords[2 * vertex01 + 1] + 
        s * object->texcoords[2 * vertex02 + 1] + 
        t * object->texcoords[2 * vertex03 + 1];

        normal[stack_index][0] = (1.f - s - t) * object->normals[3 * vertex01 + 0] + 
        s * object->normals[3 * vertex02 + 0] + 
        t * object->normals[3 * vertex03 + 0];

        normal[stack_index][1] = (1.f - s - t) * object->normals[3 * vertex01 + 1] + 
        s * object->normals[3 * vertex02 + 1] + 
        t * object->normals[3 * vertex03 + 1];

        normal[stack_index][2] = (1.f - s - t) * object->normals[3 * vertex01 + 2] + 
        s * object->normals[3 * vertex02 + 2] + 
        t * object->normals[3 * vertex03 + 2];

        Vector3f origin = Vector3f(m_batch[index].org[0], m_batch[index].org[1], m_batch[index].org[2]);
        Vector3f direction = Vector3f(m_batch[index].dir[0], m_batch[index].dir[1], m_batch[index].dir[2]).normalized();

        position[stack_index] = origin + direction * (m_batch[index].tfar - M_EPSILON);
        output[stack_index] = direction * -1.f;
      }

      // Evaluate light
      light->illuminate(range, &random, position, input, radiance);

      // Evaluate shader
      shader->evaluate(range, texture_system, input, output, normal, u, v, result);

      //Post loop through the group
      for(size_t index = r.begin(); index < r.end(); ++index)
      {
        size_t stack_index = index - r.begin();

        // Add shader colour to samples
        m_image->samples[m_batch[index].sampleID].r = result[3 * stack_index + 0] * radiance[stack_index][0];
        m_image->samples[m_batch[index].sampleID].g = result[3 * stack_index + 1] * radiance[stack_index][1];
        m_image->samples[m_batch[index].sampleID].b = result[3 * stack_index + 2] * radiance[stack_index][2];
      }

      delete[] result;
      delete[] u;
      delete[] v;
      delete[] input;
      delete[] output;
      delete[] normal;
      delete[] position;
      delete[] radiance;
    }
    // If object is a light
    else
    {
      //Loopt through the group
      for(size_t index = r.begin(); index < r.end(); ++index)
      {
        // Set samples to white
        m_image->samples[m_batch[index].sampleID].r = 1.f;
        m_image->samples[m_batch[index].sampleID].g = 1.f;
        m_image->samples[m_batch[index].sampleID].b = 1.f;
      }
    }
  }
  // If they did not hit an object
  else
  {
    //Loopt through the group
    for(size_t index = r.begin(); index < r.end(); ++index)
    {
      // Set samples to black
      m_image->samples[m_batch[index].sampleID].r = 0.f;
      m_image->samples[m_batch[index].sampleID].g = 0.f;
      m_image->samples[m_batch[index].sampleID].b = 0.f;
    }
  }
}

MSC_NAMESPACE_END