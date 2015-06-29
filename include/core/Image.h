#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <core/Common.h>
#include <core/RandomGenerator.h>

MSC_NAMESPACE_BEGIN

struct Sample
{
  float x, y;
  float r, g, b;

  Sample() {;}
  Sample(float _x, float _y, float _r, float _g, float _b)
   : x(_x), y(_y), r(_r), g(_g), b(_b) {;}
};

class Image
{
public:
  Image()
    : m_width(0)
    , m_height(0)
    , m_samples(0)
    , m_iteration(0)
  {;}

  Image(const int _width, const int _height, const int _samples);
  ~Image();

private:
  int m_width;
  int m_height;
  int m_samples;
  int m_iteration;
  RandomGenerator m_random;
  float* m_image_r;
  float* m_image_g;
  float* m_image_b;

public:
  Sample* data;
};

MSC_NAMESPACE_END

#endif
