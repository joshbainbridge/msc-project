#include <core/SurfaceThread.h>
#include <core/PolygonObject.h>

MSC_NAMESPACE_BEGIN

void SurfaceThread::start(tbb::concurrent_queue< SurfaceTask >* _queue, RayUncompressed* _batch)
{
  m_thread = boost::thread(&SurfaceThread::process, this, _queue, _batch);
}

void SurfaceThread::join()
{
  m_thread.join();
}

void SurfaceThread::process(tbb::concurrent_queue< SurfaceTask >* _queue, RayUncompressed* _batch)
{
  SurfaceTask task;

  while(_queue->try_pop(task))
  {
    if(_batch[task.begin].geomID != -1)
    {
      PolygonObject* object = &(m_scene->objects[_batch[task.begin].geomID]);

      for(size_t iterator = task.begin; iterator < task.end; ++iterator)
      {
        size_t vertex01 = object->indices[3 * _batch[iterator].primID + 0];
        size_t vertex02 = object->indices[3 * _batch[iterator].primID + 1];
        size_t vertex03 = object->indices[3 * _batch[iterator].primID + 2];

        float u = _batch[iterator].u;
        float v = _batch[iterator].v;

        float x = 
        (1.f - u - v) * object->texcoords[2 * vertex01 + 0] + 
        u * object->texcoords[2 * vertex02 + 0] + 
        v * object->texcoords[2 * vertex03 + 0];

        float y = 
        (1.f - u - v) * object->texcoords[2 * vertex01 + 1] + 
        u * object->texcoords[2 * vertex02 + 1] + 
        v * object->texcoords[2 * vertex03 + 1];

        m_image->samples[_batch[iterator].sampleID].r = x;
        m_image->samples[_batch[iterator].sampleID].g = y;
        m_image->samples[_batch[iterator].sampleID].b = 0.f;
      }
    }
    else
    {
      for(size_t iterator = task.begin; iterator < task.end; ++iterator)
      {
        m_image->samples[_batch[iterator].sampleID].r = 0.f;
        m_image->samples[_batch[iterator].sampleID].g = 0.f;
        m_image->samples[_batch[iterator].sampleID].b = 0.f;
      }
    }
  }
}

MSC_NAMESPACE_END