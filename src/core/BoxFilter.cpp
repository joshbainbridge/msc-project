#include <core/BoxFilter.h>

MSC_NAMESPACE_BEGIN

void BoxFilter::convolve(const size_t _width, const size_t _height, const size_t _samples, Sample* _input, Pixel* _output) const
{ 
  for(size_t iterator_x = 0; iterator_x < _width; ++iterator_x)
  {
    for(size_t iterator_y = 0; iterator_y < _height; ++iterator_y)
    {
      Colour3f summation(0.f, 0.f, 0.f);

      for(size_t iterator = 0; iterator < _samples; ++iterator)
      {
        size_t sample_index = (iterator_x * _height * _samples) + (iterator_y * _samples) + (iterator);
        summation[0] += _input[sample_index].r;
        summation[1] += _input[sample_index].g;
        summation[2] += _input[sample_index].b;
      }

      size_t pixel_index = iterator_y * _width + iterator_x;
      _output[pixel_index].r += (summation[0] / _samples);
      _output[pixel_index].g += (summation[1] / _samples);
      _output[pixel_index].b += (summation[2] / _samples);
    }
  }
}

MSC_NAMESPACE_END