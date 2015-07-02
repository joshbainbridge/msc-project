#ifndef _SAMPLERINTERFACE_H_
#define _SAMPLERINTERFACE_H_

#include <core/Common.h>

MSC_NAMESPACE_BEGIN

class SamplerInterface
{
public:
  virtual ~SamplerInterface() {}

  virtual void virtualFunc(const float value_one, const float value_two) const =0;

protected:
  float m_variable_one;
  float m_variable_two;
};

MSC_NAMESPACE_END

#endif
