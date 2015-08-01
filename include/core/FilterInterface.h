#ifndef _FILTERINTERFACE_H_
#define _FILTERINTERFACE_H_

#include <core/Common.h>
#include <core/Image.h>

MSC_NAMESPACE_BEGIN

/**
 * @brief      Abstract interface class for image filtering
 */
class FilterInterface
{
public:
  virtual ~FilterInterface() {}

  /**
   * @brief      Convolution function for producing final image
   *
   * @param[in]  _width       width of image in pixels
   * @param[in]  _height      height of image in pixels
   * @param[in]  _samples     maximum samples
   * @param[in]  _rows_begin  begining of range to be calcualted in the vertical direction
   * @param[in]  _rows_end    end of range to be calcualted in the vertical direction
   * @param[in]  _cols_begin  begining of range to be calcualted in the horizontal direction
   * @param[in]  _cols_end    end of range to be calcualted in the horizontal direction
   * @param      _input       input sample data
   * @param      _output      output image
   */
  virtual void convolve(
    const size_t _width,
    const size_t _height,
    const size_t _samples,
    const size_t _rows_begin,
    const size_t _rows_end,
    const size_t _cols_begin,
    const size_t _cols_end,
    Sample* _input,
    Pixel* _output
    ) const =0;
};

MSC_NAMESPACE_END

#endif
