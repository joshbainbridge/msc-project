#ifndef _CONVOLVE_H_
#define _CONVOLVE_H_

#include <tbb/blocked_range2d.h>

#include <core/Common.h>
#include <core/Image.h>
#include <core/FilterInterface.h>

MSC_NAMESPACE_BEGIN

/**
 * @brief      Used to filter the final image from sample data
 */
class Convolve
{
public:
  Convolve(FilterInterface* _filter, Image* _image)
   : m_filter(_filter)
   , m_image(_image)
  {;}

  /**
   * @brief      Operator overloader to allow the class to act as a functor with tbb
   * 
   * @param[in]  r           a two dimentional blocked range over image resolution
   */
  void operator()(const tbb::blocked_range2d< size_t > &r) const;

private:
  FilterInterface* m_filter;
  Image* m_image;
};

MSC_NAMESPACE_END

#endif