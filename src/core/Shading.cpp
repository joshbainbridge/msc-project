#include <map>
#include <vector>
#include <iostream>

#include <core/Shading.h>
#include <core/PolygonObject.h>
#include <core/ShaderInterface.h>
#include <core/LightInterface.h>

MSC_NAMESPACE_BEGIN

void Shading::operator()(const RangeGeom< RayUncompressed* > &r) const
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

    std::map<int, int>::const_iterator it = m_scene->shaders_to_lights.find(object->shader);
    if(it != m_scene->shaders_to_lights.end())
      light_id = it->second;

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
      for(size_t iterator = r.begin(); iterator < r.end(); ++iterator)
      {
        size_t index = iterator - r.begin();

        // Find the index to each vertex of the intersected primitive
        size_t vertex01 = object->indices[3 * m_batch[iterator].primID + 0];
        size_t vertex02 = object->indices[3 * m_batch[iterator].primID + 1];
        size_t vertex03 = object->indices[3 * m_batch[iterator].primID + 2];

        // The primitives barycentric coordinates
        float s = m_batch[iterator].u;
        float t = m_batch[iterator].v;

        // The intersected texture coordinates
        u[index] = (1.f - s - t) * object->texcoords[2 * vertex01 + 0] + 
        s * object->texcoords[2 * vertex02 + 0] + 
        t * object->texcoords[2 * vertex03 + 0];

        v[index] = (1.f - s - t) * object->texcoords[2 * vertex01 + 1] + 
        s * object->texcoords[2 * vertex02 + 1] + 
        t * object->texcoords[2 * vertex03 + 1];

        normal[index][0] = (1.f - s - t) * object->normals[3 * vertex01 + 0] + 
        s * object->normals[3 * vertex02 + 0] + 
        t * object->normals[3 * vertex03 + 0];

        normal[index][1] = (1.f - s - t) * object->normals[3 * vertex01 + 1] + 
        s * object->normals[3 * vertex02 + 1] + 
        t * object->normals[3 * vertex03 + 1];

        normal[index][2] = (1.f - s - t) * object->normals[3 * vertex01 + 2] + 
        s * object->normals[3 * vertex02 + 2] + 
        t * object->normals[3 * vertex03 + 2];

        Vector3f origin = Vector3f(m_batch[iterator].org[0], m_batch[iterator].org[1], m_batch[iterator].org[2]);
        Vector3f direction = Vector3f(m_batch[iterator].dir[0], m_batch[iterator].dir[1], m_batch[iterator].dir[2]).normalized();

        position[index] = origin + direction * (m_batch[iterator].tfar - M_EPSILON);
        output[index] = direction * -1.f;
      }

      // Evaluate light
      light->sample(range, &random, position, input, radiance);

      // Evaluate shader
      shader->evaluate(range, texture_system, input, output, normal, u, v, result);

      //Post loop through the group
      for(size_t iterator = r.begin(); iterator < r.end(); ++iterator)
      {
        size_t index = iterator - r.begin();

        // Add shader colour to samples
        m_image->samples[m_batch[iterator].sampleID].r = result[3 * index + 0] * radiance[index][0];
        m_image->samples[m_batch[iterator].sampleID].g = result[3 * index + 1] * radiance[index][1];
        m_image->samples[m_batch[iterator].sampleID].b = result[3 * index + 2] * radiance[index][2];
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
      for(size_t iterator = r.begin(); iterator < r.end(); ++iterator)
      {
        // Set samples to white
        m_image->samples[m_batch[iterator].sampleID].r = 1.f;
        m_image->samples[m_batch[iterator].sampleID].g = 1.f;
        m_image->samples[m_batch[iterator].sampleID].b = 1.f;
      }
    }
  }
  // If they did not hit an object
  else
  {
    //Loopt through the group
    for(size_t iterator = r.begin(); iterator < r.end(); ++iterator)
    {
      // Set samples to black
      m_image->samples[m_batch[iterator].sampleID].r = 0.f;
      m_image->samples[m_batch[iterator].sampleID].g = 0.f;
      m_image->samples[m_batch[iterator].sampleID].b = 0.f;
    }
  }
}

MSC_NAMESPACE_END