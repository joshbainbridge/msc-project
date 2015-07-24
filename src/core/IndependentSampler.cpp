#include <core/IndependentSampler.h>

MSC_NAMESPACE_BEGIN

void IndependentSampler::sample(const int _base, RandomGenerator* _random, float* _output) const
{
  for(size_t iterator_x = 0; iterator_x < _base; ++iterator_x)
  {
    for(size_t iterator_y = 0; iterator_y < _base; ++iterator_y)
    {
      _output[2 * (iterator_y + iterator_x * _base) + 0] = _random->getSample();
      _output[2 * (iterator_y + iterator_x * _base) + 1] = _random->getSample();
    }
  }
}

MSC_NAMESPACE_END