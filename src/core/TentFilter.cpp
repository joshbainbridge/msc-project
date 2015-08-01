#include <core/TentFilter.h>

MSC_NAMESPACE_BEGIN

void TentFilter::convolve(
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
  for(int index_pixel_x = _rows_begin; index_pixel_x < _rows_end; ++index_pixel_x)
  {
    for(int index_pixel_y = _cols_begin; index_pixel_y < _cols_end; ++index_pixel_y)
    {
      Colour3f summation(0.f, 0.f, 0.f);
      float center_pos_x = index_pixel_x + 0.5f;
      float center_pos_y = index_pixel_y + 0.5f;

      float denominator = 0;

      for(int index_mask_x = -1; index_mask_x < 2; ++index_mask_x)
      {
        for(int index_mask_y = -1; index_mask_y < 2; ++index_mask_y)
        {
          int pixel_x = index_pixel_x + index_mask_x;
          int pixel_y = index_pixel_y + index_mask_y;
          
          if(!(pixel_x < 0) && (pixel_x < _width) && !(pixel_y < 0) && (pixel_y < _height))
          {
            for(size_t index_sample = 0; index_sample < _samples; ++index_sample)
            {
              size_t sample_index = (pixel_x * _height * _samples) + (pixel_y * _samples) + (index_sample);
              
              float sample_pos_x = _input[sample_index].x;
              float sample_pos_y = _input[sample_index].y;
              float distance_x = fmax(0.f, 1.f - fabsf(sample_pos_x - center_pos_x));
              float distance_y = fmax(0.f, 1.f - fabsf(sample_pos_y - center_pos_y));

              float multiplier = distance_x * distance_y;

              denominator = denominator + multiplier;

              summation[0] += _input[sample_index].r * multiplier;
              summation[1] += _input[sample_index].g * multiplier;
              summation[2] += _input[sample_index].b * multiplier;
            }
          }
        }
      }

      size_t pixel_index = index_pixel_y * _width + index_pixel_x;
      _output[pixel_index].r += (summation[0] / denominator);
      _output[pixel_index].g += (summation[1] / denominator);
      _output[pixel_index].b += (summation[2] / denominator);
    }
  }
}

MSC_NAMESPACE_END