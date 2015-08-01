#ifndef _GRIDSAMPLER_H_
#define _GRIDSAMPLER_H_

#include <core/Common.h>
#include <core/SamplerInterface.h>

MSC_NAMESPACE_BEGIN

/**
 * @brief      Inherits from the sampler interface and represents a grid sampler function
 */
class GridSampler : public SamplerInterface
{
public:
  
  /**
   * @brief      Create a grid of samples across pixel area
   *
   * @param[in]  _base    base of sample count
   * @param      _random  thread local random generator to prevent mutation
   * @param      _output  output array of samples
   */
  void sample(const int _base, RandomGenerator* _random, float* _output) const;
};

MSC_NAMESPACE_END

YAML_NAMESPACE_BEGIN

template<> struct convert<msc::GridSampler>
{
  static Node encode(const msc::GridSampler& rhs)
  {
    Node node;
    node["sampler"]["type"] = "Grid";
    return node;
  }

  static bool decode(const Node& node, msc::GridSampler& rhs)
  {
    if(!node.IsMap() || node.size() != 1)
      return false;
    
    return true;
  }
};

YAML_NAMESPACE_END

#endif
