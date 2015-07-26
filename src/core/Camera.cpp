#include <core/Camera.h>
#include <core/RayCompressed.h>

MSC_NAMESPACE_BEGIN

void Camera::operator()(const tbb::blocked_range2d< size_t > &r) const
{
  LocalRandomGenerator::reference random = m_local_thread_storage->local();

  size_t count = m_image->base * m_image->base;

  float* samples = new float[count * 2];
  RayCompressed* rays = new RayCompressed[count];

  for(size_t iterator_x = r.rows().begin(); iterator_x < r.rows().end(); ++iterator_x)
  {
    for(size_t iterator_y = r.cols().begin(); iterator_y < r.cols().end(); ++iterator_y)
    {
      m_sampler->sample(m_image->base, &random, samples);

      for(size_t iterator = 0; iterator < count; ++iterator)
      {
        rays[iterator].sampleID = (iterator_x * m_image->height * count) + (iterator_y * count) + iterator;
        m_image->samples[rays[iterator].sampleID].x = iterator_x + samples[2 * iterator + 0];
        m_image->samples[rays[iterator].sampleID].y = iterator_y + samples[2 * iterator + 1];
        samples[2 * iterator + 0] = (((iterator_x + samples[2 * iterator + 0]) * 2.f - m_image->width) / m_image->width) * 36.f;
        samples[2 * iterator + 1] = (((iterator_y + samples[2 * iterator + 1]) * 2.f - m_image->height) / m_image->width) * 36.f;
      }

      m_camera->sample(count, samples, &random, rays);

      for(size_t iterator = 0; iterator < count; ++iterator)
      {
        int max = (fabs(rays[iterator].dir[0]) < fabs(rays[iterator].dir[1])) ? 1 : 0;
        int axis = (fabs(rays[iterator].dir[max]) < fabs(rays[iterator].dir[2])) ? 2 : max;
        int cardinal = (rays[iterator].dir[axis] < 0.f) ? axis : axis + 3;

        m_buffer.direction[cardinal].push_back(rays[iterator]);
      }
    }
  }

  for(size_t iterator = 0; iterator < 6; ++iterator)
  {
    if(m_buffer.direction[iterator].size() > 0)
      m_bins->add(m_buffer.direction[iterator].size(), iterator, &(m_buffer.direction[iterator][0]), m_batch_queue);
  }

  delete[] samples;
  delete[] rays;
}

MSC_NAMESPACE_END