#include <cmath>

#include <core/Image.h>

MSC_NAMESPACE_BEGIN

//Set image resolution
void Image::resolution(const int _resolution_x, const int _resolution_y)
{
  m_resolution_x = _resolution_x;
  m_resolution_y = _resolution_y;

  m_pixels.clear();
  m_pixels.resize(m_resolution_x);

  for (int column = 0; column < m_resolution_x; ++column)
  {
    m_pixels[column].resize(m_resolution_y);
  }
}

//Add sample to specific pixel in image
void Image::addSample(const int _pixel_x, const int _pixel_y, const Colour3f _sample)
{
  m_mutex.lock();
  m_pixels[_pixel_x][_pixel_y].addSample(_sample);
  m_mutex.unlock();
}

//Add bucket of sample information to image to optimise synchronisation
void Image::addBucket(const std::vector< std::vector< std::vector< Colour3f > > > &_bucket)
{
  m_mutex.lock();
  
  // for (int x_index = 0; x_index < _task.width; ++x_index)
  // {
  //   for (int y_index = 0; y_index < _task.height; ++y_index)
  //   {
  //     for (int sample_index = 0; sample_index < _task.samples; ++sample_index)
  //     {
  //       m_pixels[_task.pixel_x + x_index][_task.pixel_y + y_index].addSample(_bucket[x_index][y_index][sample_index]);
  //     }
  //   }
  // }
  
  m_mutex.unlock();
}

//Return image as a vector of colours, averaging samples from each pixel
std::vector< std::vector< Colour3f > > Image::getImage()
{
  std::vector< std::vector< Colour3f > > image_output;

  m_mutex.lock();
  for (int x_pixel = 0; x_pixel < m_resolution_x; ++x_pixel)
  {
    std::vector< Colour3f > column;

    for (int y_pixel = 0; y_pixel < m_resolution_y; ++y_pixel)
    {
      Vector3f pixel;

      pixel = m_pixels[x_pixel][y_pixel].pixelColour();

      pixel.x() = gamma(clamp(pixel.x(), 0.f, 1.f), 2.2f);
      pixel.y() = gamma(clamp(pixel.y(), 0.f, 1.f), 2.2f);
      pixel.z() = gamma(clamp(pixel.z(), 0.f, 1.f), 2.2f);

      column.push_back(pixel);
    }

    image_output.push_back(column);
  }
  m_mutex.unlock();

  return image_output;
}

//Clear all image pixel data
void Image::clearImage()
{
  m_pixels.clear();
}

//Gamma correction
float Image::gamma(const float _value, const float _gamma) const
{
  return std::pow(_value, (1.f / _gamma));
}

//Clamp colour data
float Image::clamp(const float _value, const float _min, const float _max) const
{
  return std::max(_min, std::min(_value, _max));
}

MSC_NAMESPACE_END