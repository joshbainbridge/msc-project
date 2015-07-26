#ifndef _CONVOLVE_H_
#define _CONVOLVE_H_

#include <tbb/blocked_range2d.h>

#include <core/Common.h>
#include <core/Image.h>
#include <core/FilterInterface.h>

MSC_NAMESPACE_BEGIN

class Convolve
{
public:
  Convolve(FilterInterface* _filter, Image* _image)
   : m_filter(_filter)
   , m_image(_image)
  {;}

  void operator()(const tbb::blocked_range2d< size_t > &r) const;

private:
  FilterInterface* m_filter;
  Image* m_image;
};

MSC_NAMESPACE_END

#endif