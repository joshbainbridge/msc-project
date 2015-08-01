#include <core/BoxFilter.h>

MSC_NAMESPACE_BEGIN

void BoxFilter::convolve(
  const size_t _width,
  const size_t _height,
  const size_t _samples,
  const size_t _rows_begin,
  const size_t _rows_end,
  const size_t _cols_begin,
  const size_t _cols_end,
  Sample* _input,
  Pixel* _output
  ) const
{ 
  for(size_t index_x = _rows_begin; index_x < _rows_end; ++index_x)
  {
    for(size_t index_y = _cols_begin; index_y < _cols_end; ++index_y)
    {
      Colour3f summation(0.f, 0.f, 0.f);

      for(size_t index = 0; index < _samples; ++index)
      {
        size_t sample_index = (index_x * _height * _samples) + (index_y * _samples) + (index);
        summation[0] += _input[sample_index].r;
        summation[1] += _input[sample_index].g;
        summation[2] += _input[sample_index].b;
      }

      size_t pixel_index = index_y * _width + index_x;
      _output[pixel_index].r += (summation[0] / _samples);
      _output[pixel_index].g += (summation[1] / _samples);
      _output[pixel_index].b += (summation[2] / _samples);
    }
  }
}

MSC_NAMESPACE_END