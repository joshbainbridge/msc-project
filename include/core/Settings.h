#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#include <core/Common.h>

MSC_NAMESPACE_BEGIN

/**
 * @brief      Structure of settings information
 * 
 * The settings that are read from the scene file are stored here and mostly address limits on ray
 * depth when rendering and path termination when using russian roulette. It also contains information
 * on the amount of memory to be allocated when processing different operations. Most notable of these
 * is the bin exponent that controls the size of the batches.
 */
struct Settings
{
  size_t min_depth;
  size_t max_depth;
  float threshold;
  size_t bucket_size;
  size_t shading_size;
  size_t bin_exponent;
};

MSC_NAMESPACE_END

YAML_NAMESPACE_BEGIN

template<> struct convert<msc::Settings>
{
  static bool decode(const Node& node, msc::Settings& rhs)
  {
    if(!node.IsMap() || node.size() != 6)
      return false;

    rhs.min_depth = node["min depth"].as<int>();
    rhs.max_depth = node["max depth"].as<int>();
    rhs.threshold = node["threshold"].as<float>();
    rhs.bucket_size = node["bucket size"].as<int>();
    rhs.shading_size = node["shading size"].as<int>();
    rhs.bin_exponent = node["bin exponent"].as<int>();
    return true;
  }
};

YAML_NAMESPACE_END

#endif
