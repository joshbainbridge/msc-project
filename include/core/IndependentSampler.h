#ifndef _INDEPENDENTSAMPLER_H_
#define _INDEPENDENTSAMPLER_H_

#include <core/Common.h>
#include <core/SamplerInterface.h>

MSC_NAMESPACE_BEGIN

class IndependentSampler : public SamplerInterface
{
public:
  void sample(const int _base, RandomGenerator* _random, float* _output) const;
};

MSC_NAMESPACE_END

YAML_NAMESPACE_BEGIN

template<> struct convert<msc::IndependentSampler>
{
  static Node encode(const msc::IndependentSampler& rhs)
  {
    Node node;
    node["sampler"]["type"] = "Independent";
    return node;
  }

  static bool decode(const Node& node, msc::IndependentSampler& rhs)
  {
    if(!node.IsMap() || node.size() != 1)
      return false;
    
    return true;
  }
};

YAML_NAMESPACE_END

#endif
