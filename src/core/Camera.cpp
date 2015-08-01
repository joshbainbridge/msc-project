#include <core/Camera.h>
#include <core/RayCompressed.h>

MSC_NAMESPACE_BEGIN

void Camera::operator()(const tbb::blocked_range2d< size_t > &r) const
{
  LocalRandomGenerator::reference random = m_local_thread_storage->local();

  size_t count = m_image->base * m_image->base;

  float* samples = new float[count * 2];
  RayCompressed* rays = new RayCompressed[count];

  for(size_t index_x = r.rows().begin(); index_x < r.rows().end(); ++index_x)
  {
    for(size_t index_y = r.cols().begin(); index_y < r.cols().end(); ++index_y)
    {
      m_sampler->sample(m_image->base, &random, samples);

      for(size_t index = 0; index < count; ++index)
      {
        rays[index].sampleID = (index_x * m_image->height * count) + (index_y * count) + index;
        m_image->samples[rays[index].sampleID].x = index_x + samples[2 * index + 0];
        m_image->samples[rays[index].sampleID].y = index_y + samples[2 * index + 1];
        samples[2 * index + 0] = (((index_x + samples[2 * index + 0]) * 2.f - m_image->width) / m_image->width) * 36.f;
        samples[2 * index + 1] = (((index_y + samples[2 * index + 1]) * 2.f - m_image->height) / m_image->width) * 36.f;
      }

      m_camera->sample(count, samples, &random, rays);

      for(size_t index = 0; index < count; ++index)
      {
        int max = (fabs(rays[index].dir[0]) < fabs(rays[index].dir[1])) ? 1 : 0;
        int axis = (fabs(rays[index].dir[max]) < fabs(rays[index].dir[2])) ? 2 : max;
        int cardinal = (rays[index].dir[axis] < 0.f) ? axis : axis + 3;

        m_buffer.direction[cardinal].push_back(rays[index]);
      }
    }
  }

  for(size_t index = 0; index < 6; ++index)
  {
    if(m_buffer.direction[index].size() > 0)
      m_bins->add(m_buffer.direction[index].size(), index, &(m_buffer.direction[index][0]), m_batch_queue);
  }

  delete[] samples;
  delete[] rays;
}

MSC_NAMESPACE_END