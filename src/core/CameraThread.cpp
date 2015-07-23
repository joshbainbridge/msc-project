#include <core/CameraThread.h>
#include <core/RayCompressed.h>

MSC_NAMESPACE_BEGIN

void CameraThread::start(
  DirectionalBins* _bin,
  CameraInterface* _camera,
  SamplerInterface* _sampler,
  Image* _image,
  tbb::concurrent_queue< CameraTask >* _camera_queue,
  tbb::concurrent_queue< BatchItem >* _batch_queue
  )
{
  m_thread = boost::thread(&CameraThread::process, this, _bin, _camera, _sampler, _image, _camera_queue, _batch_queue);
}

void CameraThread::join()
{
  m_thread.join();
}

void CameraThread::process(
  DirectionalBins* _bin,
  CameraInterface* _camera,
  SamplerInterface* _sampler,
  Image* _image,
  tbb::concurrent_queue< CameraTask >* _camera_queue,
  tbb::concurrent_queue< BatchItem >* _batch_queue
  )
{
  size_t count = _image->base * _image->base;

  float* samples = new float[count * 2];
  RayCompressed* rays = new RayCompressed[count];

  CameraTask task;
  while(_camera_queue->try_pop(task))
  {
    for(size_t iterator_x = task.begin_x; iterator_x < task.end_x; ++iterator_x)
    {
      for(size_t iterator_y = task.begin_y; iterator_y < task.end_y; ++iterator_y)
      {
        _sampler->sample(_image->base, &m_random, samples);

        for(size_t iterator = 0; iterator < count; ++iterator)
        {
          samples[2 * iterator + 0] = (((iterator_x + samples[2 * iterator + 0]) * 2.f - _image->width) / _image->width) * 36.f;
          samples[2 * iterator + 1] = (((iterator_y + samples[2 * iterator + 1]) * 2.f - _image->height) / _image->width) * 36.f;
          rays[iterator].sampleID = (iterator_x * _image->height * count) + (iterator_y * count) + iterator;
          _image->samples[rays[iterator].sampleID].x = samples[2 * iterator + 0];
          _image->samples[rays[iterator].sampleID].y = samples[2 * iterator + 1];
        }

        _camera->sample(count, samples, &m_random, rays);

        for(size_t iterator = 0; iterator < count; ++iterator)
        {
          int max = (fabs(rays[iterator].dir[0]) < fabs(rays[iterator].dir[1])) ? 1 : 0;
          int axis = (fabs(rays[iterator].dir[max]) < fabs(rays[iterator].dir[2])) ? 2 : max;
          int cardinal = (rays[iterator].dir[axis] < 0.f) ? axis : axis + 3;

          if(m_buffer.index[cardinal] == m_buffer.size)
          {
            _bin->add(m_buffer.size, cardinal, &(m_buffer.direction[cardinal][0]), _batch_queue);
            m_buffer.index[cardinal] = 0;
          }

          m_buffer.direction[cardinal][m_buffer.index[cardinal]] = rays[iterator];
          m_buffer.index[cardinal] += 1;
        }
      }
    }
  }

  for(size_t iterator = 0; iterator < 6; ++iterator)
  {
    _bin->add(m_buffer.size, iterator, &(m_buffer.direction[iterator][0]), _batch_queue);
    m_buffer.index[iterator] = 0;
  }

  delete[] samples;
  delete[] rays;
}

MSC_NAMESPACE_END