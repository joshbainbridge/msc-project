#include <map>
#include <vector>

#include <core/Shading.h>
#include <core/PolygonObject.h>
#include <core/ShaderInterface.h>

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

      size_t range = r.end() - r.begin();

      float* result = new float[range * 3];
      float* u = new float[range];
      float* v = new float[range];
      Vector3f* input = new Vector3f[range];
      Vector3f* output = new Vector3f[range];
      Vector3f* normal = new Vector3f[range];

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
      }

      // Evaluate shader
      shader->evaluate(range, texture_system, input, output, normal, u, v, result);

      //Post loop through the group
      for(size_t iterator = r.begin(); iterator < r.end(); ++iterator)
      {
        // Add shader colour to samples
        m_image->samples[m_batch[iterator].sampleID].r = result[3 * (iterator - r.begin()) + 0];
        m_image->samples[m_batch[iterator].sampleID].g = result[3 * (iterator - r.begin()) + 1];
        m_image->samples[m_batch[iterator].sampleID].b = result[3 * (iterator - r.begin()) + 2];
      }

      delete[] result;
      delete[] u;
      delete[] v;
      delete[] input;
      delete[] output;
      delete[] normal;
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