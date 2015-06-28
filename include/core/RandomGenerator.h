#ifndef _RANDOMGENERATOR_H_
#define _RANDOMGENERATOR_H_

#include <boost/random.hpp>

#include <core/Common.h>

MSC_NAMESPACE_BEGIN

class RandomGenerator
{
public:
  float getSample();

private:
  boost::mt19937 m_generator;
  boost::uniform_real<float> m_uniform_dist;
};

MSC_NAMESPACE_END

#endif
