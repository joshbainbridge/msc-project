#include <map>

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

    // If object is not a light
    if(light_id == -1)
    {
      ShaderInterface* shader = m_scene->shaders[object->shader].get();

      //Loopt through the group
      for(size_t iterator = r.begin(); iterator < r.end(); ++iterator)
      {
        // Find the index to each vertex of the intersected primitive
        size_t vertex01 = object->indices[3 * m_batch[iterator].primID + 0];
        size_t vertex02 = object->indices[3 * m_batch[iterator].primID + 1];
        size_t vertex03 = object->indices[3 * m_batch[iterator].primID + 2];

        // The primitives barycentric coordinates
        float s = m_batch[iterator].u;
        float t = m_batch[iterator].v;

        // The intersected texture coordinates
        float u = 
        (1.f - s - t) * object->texcoords[2 * vertex01 + 0] + 
        s * object->texcoords[2 * vertex02 + 0] + 
        t * object->texcoords[2 * vertex03 + 0];

        float v = 
        (1.f - s - t) * object->texcoords[2 * vertex01 + 1] + 
        s * object->texcoords[2 * vertex02 + 1] + 
        t * object->texcoords[2 * vertex03 + 1];

        // Evaluate shader
        Colour3f weight(0.5f, 0.5f, 0.5f);
        shader->evaluate(Vector3f(), Vector3f(), Vector3f(), texture_system, u, v, &weight);

        // Add shader colour to samples
        m_image->samples[m_batch[iterator].sampleID].r = weight.x();
        m_image->samples[m_batch[iterator].sampleID].g = weight.y();
        m_image->samples[m_batch[iterator].sampleID].b = weight.z();
      }
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