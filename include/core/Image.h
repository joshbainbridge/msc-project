#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <vector>

#include <core/Common.h>

MSC_NAMESPACE_BEGIN

/**
 * @brief      Sample structure representing colour and position on image
 * 
 * A single sample with minimum data as to conserve system memory when using large sample counts.
 */
struct Sample
{
  float x, y;
  float r, g, b;
};

/**
 * @brief      Union data type of a rgb spectral representation
 * 
 * A single pixel that uses a tri-band representation in red, green and blue of the spectral energy
 * being integrated across the surface area.
 */
union Pixel
{
  struct {float r, g, b;};
  float v[3];
};

/**
 * @brief      Image structure that contains sample and pixel data
 * 
 * Basic image structure that contains width and height information as well as the base value for
 * the sample count. The iteration count represents how many times each pixel has received an
 * approximation of the lighting integral. The data itself is stored in std::vectors to allow
 * for automatic clean up.
 */
struct Image
{
  size_t width;
  size_t height;
  size_t base;
  size_t iteration;

  std::vector< Sample > samples;
  std::vector< Pixel > pixels;
};

MSC_NAMESPACE_END

YAML_NAMESPACE_BEGIN

template<> struct convert<msc::Image>
{
  static bool decode(const Node& node, msc::Image& rhs)
  {
    if(!node.IsMap() || node.size() != 3)
      return false;

    rhs.width = node["width"].as<int>();
    rhs.height = node["height"].as<int>();
    rhs.base = node["sample base"].as<int>();
    rhs.iteration = 0;
    return true;
  }
};

YAML_NAMESPACE_END

#endif
