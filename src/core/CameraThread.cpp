#include <core/CameraThread.h>
#include <core/RayCompressed.h>

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
  RayCompressed* rays = new RayCompressed[count];

  while(_queue->try_pop(task))
  {
    for(size_t iterator_x = task.begin_x; iterator_x < task.end_x; ++iterator_x)
    {
      for(size_t iterator_y = task.begin_y; iterator_y < task.end_y; ++iterator_y)
      {
        m_sampler->sample(m_image->base, &m_random, samples);

        for(size_t iterator = 0; iterator < count; ++iterator)
        {
          samples[2 * iterator + 0] = (((iterator_x + samples[2 * iterator + 0]) * 2.f - m_image->width) / m_image->width) * 36.f;
          samples[2 * iterator + 1] = (((iterator_y + samples[2 * iterator + 1]) * 2.f - m_image->height) / m_image->width) * 36.f;
          rays[iterator].sampleID = (iterator_x * m_image->height * count) + (iterator_y * count) + iterator;
          m_image->samples[rays[iterator].sampleID].x = samples[2 * iterator + 0];
          m_image->samples[rays[iterator].sampleID].y = samples[2 * iterator + 1];
        }

        m_camera->sample(count, samples, &m_random, rays);

        for(size_t iterator = 0; iterator < count; ++iterator)
        {
          int max = (fabs(rays[iterator].dir[0]) < fabs(rays[iterator].dir[1])) ? 1 : 0;
          int axis = (fabs(rays[iterator].dir[max]) < fabs(rays[iterator].dir[2])) ? 2 : max;
          int cardinal = (rays[iterator].dir[axis] < 0.f) ? axis : axis + 3;

          if(m_local_bin->index[cardinal] == m_local_bin->size)
          {
            m_batch->add(m_local_bin->size, cardinal, &(m_local_bin->bin[cardinal][0]));
            m_local_bin->index[cardinal] = 0;
          }

          m_local_bin->bin[cardinal][m_local_bin->index[cardinal]] = rays[iterator];
          m_local_bin->index[cardinal] += 1;
        }
      }
    }
  }

  delete[] samples;
  delete[] rays;
}

MSC_NAMESPACE_END