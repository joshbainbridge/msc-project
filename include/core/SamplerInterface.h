#ifndef _SAMPLERINTERFACE_H_
#define _SAMPLERINTERFACE_H_

#include <core/Common.h>
#include <core/RandomGenerator.h>

MSC_NAMESPACE_BEGIN

class SamplerInterface
{
public:
  virtual ~SamplerInterface() {}

  virtual void sample(const int _base, RandomGenerator* _random, float* _output) const =0;
};

MSC_NAMESPACE_END

#endif
