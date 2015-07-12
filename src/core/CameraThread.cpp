#include <core/CameraThread.h>
#include <core/Ray.h>

MSC_NAMESPACE_BEGIN

void CameraThread::start(tbb::concurrent_queue< CameraTask >* _queue)
{
  m_thread = boost::thread(&CameraThread::process, this, _queue);
}

void CameraThread::join()
{
  m_thread.join();
}

void CameraThread::process(tbb::concurrent_queue< CameraTask >* _queue)
{
  CameraTask task;

  size_t count = m_image->base * m_image->base;

  float* samples = new float[count * 2];
  float* positions = new float[count * 2];
  Ray* rays = new Ray[count];

  while(_queue->try_pop(task))
  {
    for(size_t iterator_x = task.begin_x; iterator_x < task.end_x; ++iterator_x)
    {
      for(size_t iterator_y = task.begin_y; iterator_y < task.end_y; ++iterator_y)
      {
        m_sampler->sample(m_image->base, &m_random, samples);

        for(size_t iterator_sample = 0; iterator_sample < count; ++iterator_sample)
        {
          positions[2 * iterator_sample + 0] = ((iterator_x + samples[2 * iterator_sample + 0]) * 2.f - m_image->width) / m_image->width;
          positions[2 * iterator_sample + 1] = ((iterator_y + samples[2 * iterator_sample + 1]) * 2.f - m_image->height) / m_image->width;
        }

        m_camera->sample(count, positions, &m_random, rays);

        for(size_t iterator_rays = 0; iterator_rays < count; ++iterator_rays)
        {
          int max = (fabs(rays[iterator_rays].direction[0]) < fabs(rays[iterator_rays].direction[1])) ? 1 : 0;
          int axis = (fabs(rays[iterator_rays].direction[max]) < fabs(rays[iterator_rays].direction[2])) ? 2 : max;
          int cardinal = (rays[iterator_rays].direction[axis] < 0.f) ? axis : axis + 3;

          if(m_local_bin->index[cardinal] == m_local_bin->bin[cardinal].size())
          {
            // Add bin to global state
            m_local_bin->index[cardinal] = 0;
          }

          m_local_bin->bin[cardinal][m_local_bin->index[cardinal]] = rays[iterator_rays];
          m_local_bin->index[cardinal] += 1;
        }
      }
    }
  }

  delete[] samples;
  delete[] positions;
  delete[] rays;
}

MSC_NAMESPACE_END