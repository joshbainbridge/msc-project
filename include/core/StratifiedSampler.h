#ifndef _STRATIFIEDSAMPLER_H_
#define _STRATIFIEDSAMPLER_H_

#include <core/Common.h>
#include <core/SamplerInterface.h>

MSC_NAMESPACE_BEGIN

class StratifiedSampler : public SamplerInterface
{
public:
  StratifiedSampler();

  void virtualFunc(const float value_one, const float value_two) const;

private:
  float m_other_variable;
};

MSC_NAMESPACE_END

#endif
