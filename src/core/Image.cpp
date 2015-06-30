#include <core/Image.h>

MSC_NAMESPACE_BEGIN

Image::Image(const int _width, const int _height, const int _base)
{
  m_width = _width;
  m_height = _height;
  m_base = _base;

  size_t sample_count = _width * _height * _base;
  samples.resize(sample_count);

  size_t channel_count = _width * _height * 3;
  pixels.resize(channel_count);
  for(size_t i = 0; i < channel_count; ++i)
    pixels[i] = 0.f;
}

MSC_NAMESPACE_END