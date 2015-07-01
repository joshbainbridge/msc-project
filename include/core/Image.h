#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <vector>

#include <core/Common.h>

MSC_NAMESPACE_BEGIN

struct Sample
{
  float x, y;
  float r, g, b;
};

struct Pixel
{
  float r, g, b;
};

struct Image
{
  size_t width;
  size_t height;
  size_t base;

  std::vector< Sample > samples;
  std::vector< Pixel > pixels;
};

MSC_NAMESPACE_END

#endif
