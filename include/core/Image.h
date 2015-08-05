#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <vector>

#include <core/Common.h>

MSC_NAMESPACE_BEGIN

/**
 * @brief      Sample structure representing colour and position on image
 */
struct Sample
{
  float x, y;
  float r, g, b;
};

/**
 * @brief      Union data type of a rgb spectral representation
 */
union Pixel
{
  struct {float r, g, b;};
  float v[3];
};

/**
 * @brief      Image structure that contains sample and pixel data
 */
struct Image
{
  size_t width;
  size_t height;
  size_t base;
  size_t depth;
  size_t iteration;

  std::vector< Sample > samples;
  std::vector< Pixel > pixels;
};

MSC_NAMESPACE_END

YAML_NAMESPACE_BEGIN

template<> struct convert<msc::Image>
{
  static Node encode(const msc::Image& rhs)
  {
    Node node;
    node["image"]["width"] = rhs.width;
    node["image"]["height"] = rhs.height;
    node["image"]["sample base"] = rhs.base;
    node["image"]["ray depth"] = rhs.depth;
    return node;
  }

  static bool decode(const Node& node, msc::Image& rhs)
  {
    if(!node.IsMap() || node.size() != 4)
      return false;

    rhs.width = node["width"].as<int>();
    rhs.height = node["height"].as<int>();
    rhs.base = node["sample base"].as<int>();
    rhs.depth = node["ray depth"].as<int>();
    rhs.iteration = 0;
    return true;
  }
};

YAML_NAMESPACE_END

#endif
