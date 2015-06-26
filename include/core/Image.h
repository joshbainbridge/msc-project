#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <vector>

#include <boost/thread.hpp>

#include <core/Common.h>
#include <core/Pixel.h>

MSC_NAMESPACE_BEGIN

class Image
{
public:
  Image() : m_resolution_x(0), m_resolution_y(0) {;}

  void resolution(const int _resolution_x, const int _resolution_y);
  void addSample(const int _pixel_x, const int _pixel_y, const Colour3f _sample);
  void addBucket(const std::vector< std::vector< std::vector< Colour3f > > > &_bucket);
  std::vector< std::vector< Colour3f > > getImage();
  void clearImage();

private:
  std::vector< std::vector<Pixel> > m_pixels;
  int m_resolution_x;
  int m_resolution_y;
  boost::mutex m_mutex;

  float gamma(const float _value, const float _gamma) const;
  float clamp(const float _value, const float _min, const float _max) const;
};

MSC_NAMESPACE_END

#endif
