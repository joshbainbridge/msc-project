#include <cmath>

#include <core/Image.h>

MSC_NAMESPACE_BEGIN

Image::Image(const int _width, const int _height, const int _samples)
{
  m_width = _width;
  m_height = _height;
  m_samples = _samples;

  size_t sample_count = _width * _height * _samples;
  data = new Sample[sample_count];
  for(size_t i = 0; i < sample_count; ++i)
    data[i] = Sample(0, 0, 0.f, 0.f, 0.f);

  size_t pixel_count = _width * _height;
  m_image_r = new float[pixel_count];
  m_image_g = new float[pixel_count];
  m_image_b = new float[pixel_count];
  for(size_t i = 0; i < sample_count; ++i)
  {
    m_image_r[i] = 0.f;
    m_image_g[i] = 0.f;
    m_image_b[i] = 0.f;
  }

  m_iteration = 0;
}

Image::~Image()
{
  delete[] data;
  delete[] m_image_r;
  delete[] m_image_g;
  delete[] m_image_b;
}

MSC_NAMESPACE_END