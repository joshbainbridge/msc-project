#include <core/RandomGenerator.h>

MSC_NAMESPACE_BEGIN

//Generate uniform sample (mutates class)
float RandomGenerator::getSample()
{
  return m_uniform_dist(m_generator);
}

MSC_NAMESPACE_END