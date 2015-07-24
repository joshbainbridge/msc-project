#include <core/GridSampler.h>

MSC_NAMESPACE_BEGIN

void GridSampler::sample(const int _base, RandomGenerator* _random, float* _output) const
{
  float cell_size = 1.f / _base;

  for(size_t iterator_x = 0; iterator_x < _base; ++iterator_x)
  {
    for(size_t iterator_y = 0; iterator_y < _base; ++iterator_y)
    {
      _output[2 * (iterator_y + iterator_x * _base) + 0] = iterator_x * cell_size + 0.5f * cell_size;
      _output[2 * (iterator_y + iterator_x * _base) + 1] = iterator_y * cell_size + 0.5f * cell_size;
    }
  }
}

MSC_NAMESPACE_END