#include <core/IndependentSampler.h>

MSC_NAMESPACE_BEGIN

void IndependentSampler::sample(const int _base, RandomGenerator* _random, float* _output) const
{
  for(size_t index_x = 0; index_x < _base; ++index_x)
  {
    for(size_t index_y = 0; index_y < _base; ++index_y)
    {
      _output[2 * (index_y + index_x * _base) + 0] = _random->getSample();
      _output[2 * (index_y + index_x * _base) + 1] = _random->getSample();
    }
  }
}

MSC_NAMESPACE_END