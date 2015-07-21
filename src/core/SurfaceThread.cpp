#include <string>
#include <map>

#include <core/SurfaceThread.h>
#include <core/PolygonObject.h>
#include <core/ShaderInterface.h>

MSC_NAMESPACE_BEGIN

void SurfaceThread::start(tbb::concurrent_queue< SurfaceTask >* _queue, RayUncompressed* _batch)
{
  m_texture_system = OpenImageIO::TextureSystem::create(true);
  m_thread = boost::thread(&SurfaceThread::process, this, _queue, _batch);
}

void SurfaceThread::join()
{
  m_thread.join();
  OpenImageIO::TextureSystem::destroy(m_texture_system, false);
}

void SurfaceThread::process(tbb::concurrent_queue< SurfaceTask >* _queue, RayUncompressed* _batch)
{
  SurfaceTask task;

  // Hit point group processing
  while(_queue->try_pop(task))
  {
    // Get geometry id for group
    size_t geom_id = _batch[task.begin].geomID;

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
        for(size_t iterator = task.begin; iterator < task.end; ++iterator)
        {
          // Find the index to each vertex of the intersected primitive
          size_t vertex01 = object->indices[3 * _batch[iterator].primID + 0];
          size_t vertex02 = object->indices[3 * _batch[iterator].primID + 1];
          size_t vertex03 = object->indices[3 * _batch[iterator].primID + 2];

          // The primitives barycentric coordinates
          float s = _batch[iterator].u;
          float t = _batch[iterator].v;

          // The intersected texture coordinates
          float u = 
          (1.f - s - t) * object->texcoords[2 * vertex01 + 0] + 
          s * object->texcoords[2 * vertex02 + 0] + 
          t * object->texcoords[2 * vertex03 + 0];

          float v = 
          (1.f - s - t) * object->texcoords[2 * vertex01 + 1] + 
          s * object->texcoords[2 * vertex02 + 1] + 
          t * object->texcoords[2 * vertex03 + 1];

          // Find the texture colour
          // float result[3];
          // OpenImageIO::TextureOpt options;

          // OpenImageIO::ustring filename = OpenImageIO::ustring("scenes/textures/floor_pattern.tx");
          // m_texture_system->texture(filename, options, u, v, 0, 0, 0, 0, 3, result);
          Colour3f weight;
          shader->evaluate(Vector3f(), Vector3f(), Vector3f(), m_texture_system, u, v, &weight);

          // Add texture colour to samples
          m_image->samples[_batch[iterator].sampleID].r = weight.x();
          m_image->samples[_batch[iterator].sampleID].g = weight.y();
          m_image->samples[_batch[iterator].sampleID].b = weight.z();
        }
      }
      // If object is a light
      else
      {
        //Loopt through the group
        for(size_t iterator = task.begin; iterator < task.end; ++iterator)
        {
          // Set samples to white
          m_image->samples[_batch[iterator].sampleID].r = 1.f;
          m_image->samples[_batch[iterator].sampleID].g = 1.f;
          m_image->samples[_batch[iterator].sampleID].b = 1.f;
        }
      }
    }
    // If they did not hit an object
    else
    {
      //Loopt through the group
      for(size_t iterator = task.begin; iterator < task.end; ++iterator)
      {
        // Set samples to black
        m_image->samples[_batch[iterator].sampleID].r = 0.f;
        m_image->samples[_batch[iterator].sampleID].g = 0.f;
        m_image->samples[_batch[iterator].sampleID].b = 0.f;
      }
    }
  }
}

MSC_NAMESPACE_END