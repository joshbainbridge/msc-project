#ifndef _TENTFILTER_H_
#define _TENTFILTER_H_

#include <core/Common.h>
#include <core/FilterInterface.h>

MSC_NAMESPACE_BEGIN

/**
 * @brief      Inherits from the filter interface and represents a tent filter function
 */
class TentFilter : public FilterInterface
{
public:

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
  void convolve(
    const size_t _width,
    const size_t _height,
    const size_t _samples,
    const size_t _rows_begin,
    const size_t _rows_end,
    const size_t _cols_begin,
    const size_t _cols_end,
    Sample* _input,
    Pixel* _output
    ) const;
};

MSC_NAMESPACE_END

YAML_NAMESPACE_BEGIN

template<> struct convert<msc::TentFilter>
{
  static Node encode(const msc::TentFilter& rhs)
  {
    Node node;
    node["filter"]["type"] = "Tent";
    return node;
  }

  static bool decode(const Node& node, msc::TentFilter& rhs)
  {
    if(!node.IsMap() || node.size() != 1)
      return false;
    
    return true;
  }
};

YAML_NAMESPACE_END

#endif
