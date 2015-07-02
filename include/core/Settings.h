#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#include <core/Common.h>

MSC_NAMESPACE_BEGIN

struct Settings
{
  size_t ray_depth;
  size_t bucket_size;
  size_t bin_exponent;
  size_t batch_exponent;
  size_t iteration;
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
    node["settings"]["bin exponent"] = rhs.bin_exponent;
    node["settings"]["batch exponent"] = rhs.batch_exponent;
    return node;
  }

  static bool decode(const Node& node, msc::Settings& rhs)
  {
    if(!node.IsMap() || node.size() != 4)
    {
      return false;
    }

    rhs.ray_depth = node["ray depth"].as<int>();
    rhs.bucket_size = node["bucket size"].as<int>();
    rhs.bin_exponent = node["bin exponent"].as<int>();
    rhs.batch_exponent = node["batch exponent"].as<int>();
    rhs.iteration = 0;
    return true;
  }
};

YAML_NAMESPACE_END

#endif
