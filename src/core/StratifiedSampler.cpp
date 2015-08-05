#include <core/StratifiedSampler.h>

MSC_NAMESPACE_BEGIN

void StratifiedSampler::sample(const int _base, RandomGenerator* _random, float* _output) const
{
  float cell_size = 1.f / _base;

  for(size_t index_x = 0; index_x < _base; ++index_x)
  {
    for(size_t index_y = 0; index_y < _base; ++index_y)
    {
      _output[2 * (index_y + index_x * _base) + 0] = index_x * cell_size + _random->sample() * cell_size;
      _output[2 * (index_y + index_x * _base) + 1] = index_y * cell_size + _random->sample() * cell_size;
    }
  }
}

MSC_NAMESPACE_END