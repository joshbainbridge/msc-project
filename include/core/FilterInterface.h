#ifndef _FILTERINTERFACE_H_
#define _FILTERINTERFACE_H_

#include <core/Common.h>
#include <core/Image.h>

MSC_NAMESPACE_BEGIN

class FilterInterface
{
public:
  virtual ~FilterInterface() {}

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

protected:
  float m_variable_one;
  float m_variable_two;
};

MSC_NAMESPACE_END

#endif
