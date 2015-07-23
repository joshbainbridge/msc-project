#include <string>
#include <map>

#include <core/SurfaceThread.h>
#include <core/PolygonObject.h>
#include <core/ShaderInterface.h>

MSC_NAMESPACE_BEGIN

void SurfaceThread::start(
  DirectionalBins* _bin,
  Scene* _scene,
  Image* _image,
  tbb::concurrent_queue< SurfaceTask >* _surface_queue,
  tbb::concurrent_queue< BatchItem >* _batch_queue,
  RayUncompressed* _batch
  )
{
  m_texture_system = OpenImageIO::TextureSystem::create(true);
  m_thread = boost::thread(&SurfaceThread::process, this, _bin, _scene, _image, _surface_queue, _batch_queue, _batch);
}

void SurfaceThread::join()
{
  m_thread.join();
  OpenImageIO::TextureSystem::destroy(m_texture_system, false);
}

void SurfaceThread::process(
  DirectionalBins* _bin,
  Scene* _scene,
  Image* _image,
  tbb::concurrent_queue< SurfaceTask >* _surface_queue,
  tbb::concurrent_queue< BatchItem >* _batch_queue,
  RayUncompressed* _batch
  )
{
  // Hit point group processing
  SurfaceTask task;
  while(_surface_queue->try_pop(task))
  {
    // Get geometry id for group
    size_t geom_id = _batch[task.begin].geomID;

    // Did the points hit an object
    if(geom_id != -1)
    {
      // Which object did they hit
      PolygonObject* object = _scene->objects[geom_id].get();

      // Check if object is a light
      int light_id = -1;

      std::map<int, int>::const_iterator it = _scene->shaders_to_lights.find(object->shader);
      if(it != _scene->shaders_to_lights.end())
        light_id = it->second;

      // If object is not a light
      if(light_id == -1)
      {
        ShaderInterface* shader = _scene->shaders[object->shader].get();

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

          // Evaluate shader
          Colour3f weight;
          shader->evaluate(Vector3f(), Vector3f(), Vector3f(), m_texture_system, u, v, &weight);

          // Add shader colour to samples
          _image->samples[_batch[iterator].sampleID].r = weight.x();
          _image->samples[_batch[iterator].sampleID].g = weight.y();
          _image->samples[_batch[iterator].sampleID].b = weight.z();
        }
      }
      // If object is a light
      else
      {
        //Loopt through the group
        for(size_t iterator = task.begin; iterator < task.end; ++iterator)
        {
          // Set samples to white
          _image->samples[_batch[iterator].sampleID].r = 1.f;
          _image->samples[_batch[iterator].sampleID].g = 1.f;
          _image->samples[_batch[iterator].sampleID].b = 1.f;
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
        _image->samples[_batch[iterator].sampleID].r = 0.f;
        _image->samples[_batch[iterator].sampleID].g = 0.f;
        _image->samples[_batch[iterator].sampleID].b = 0.f;
      }
    }
  }
}

MSC_NAMESPACE_END