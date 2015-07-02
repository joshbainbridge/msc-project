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

union Pixel
{
  struct {float r, g, b;};
  float v[3];
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

YAML_NAMESPACE_BEGIN

template<> struct convert<msc::Image>
{
  static Node encode(const msc::Image& rhs)
  {
    Node node;
    node["image"]["width"] = rhs.width;
    node["image"]["height"] = rhs.height;
    node["image"]["sample base"] = rhs.base;
    return node;
  }

  static bool decode(const Node& node, msc::Image& rhs)
  {
    if(!node.IsMap() || node.size() != 3)
    {
      return false;
    }

    rhs.width = node["width"].as<int>();
    rhs.height = node["height"].as<int>();
    rhs.base = node["sample base"].as<int>();
    return true;
  }
};

YAML_NAMESPACE_END

#endif
