#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#include <core/Common.h>

MSC_NAMESPACE_BEGIN

struct Settings
{
  size_t ray_depth;
  size_t bucket_size;
  size_t shading_size;
  size_t buffer_exponent;
  size_t bin_exponent;
};

MSC_NAMESPACE_END

YAML_NAMESPACE_BEGIN

template<> struct convert<msc::Settings>
{
  static Node encode(const msc::Settings& rhs)
  {
    Node node;
    node["settings"]["ray depth"] = rhs.ray_depth;
    node["settings"]["bucket size"] = rhs.bucket_size;
    node["settings"]["shading size"] = rhs.bucket_size;
    node["settings"]["buffer exponent"] = rhs.buffer_exponent;
    node["settings"]["bin exponent"] = rhs.bin_exponent;
    return node;
  }

  static bool decode(const Node& node, msc::Settings& rhs)
  {
    if(!node.IsMap() || node.size() != 5)
      return false;

    rhs.ray_depth = node["ray depth"].as<int>();
    rhs.bucket_size = node["bucket size"].as<int>();
    rhs.shading_size = node["shading size"].as<int>();
    rhs.buffer_exponent = node["buffer exponent"].as<int>();
    rhs.bin_exponent = node["bin exponent"].as<int>();
    return true;
  }
};

YAML_NAMESPACE_END

#endif
