#ifndef _SAMPLERINTERFACE_H_
#define _SAMPLERINTERFACE_H_

#include <core/Common.h>
#include <core/RandomGenerator.h>

MSC_NAMESPACE_BEGIN

/**
 * @brief      Abstract interface class for image samplers
 */
class SamplerInterface
{
public:
  virtual ~SamplerInterface() {}

  /**
   * @brief      Create samples across pixel area
   *
   * @param[in]  _base    base of sample count
   * @param      _random  thread local random generator to prevent mutation
   * @param      _output  output array of samples
   */
  virtual void sample(const int _base, RandomGenerator* _random, float* _output) const =0;
};

MSC_NAMESPACE_END

#endif
