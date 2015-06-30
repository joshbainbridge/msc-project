#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <vector>

#include <core/Common.h>
#include <core/RandomGenerator.h>

MSC_NAMESPACE_BEGIN

struct Sample
{
  float x, y;
  float r, g, b;
};

class Image
{
public:
  Image(const int _width, const int _height, const int _base);

public:
  std::vector< Sample > samples;
  std::vector< float > pixels;

private:
  int m_width;
  int m_height;
  int m_base;

  RandomGenerator m_random;
};

MSC_NAMESPACE_END

#endif
