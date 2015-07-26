#ifndef _BOXFILTER_H_
#define _BOXFILTER_H_

#include <core/Common.h>
#include <core/FilterInterface.h>

MSC_NAMESPACE_BEGIN

class BoxFilter : public FilterInterface
{
public:
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

template<> struct convert<msc::BoxFilter>
{
  static Node encode(const msc::BoxFilter& rhs)
  {
    Node node;
    node["filter"]["type"] = "Box";
    return node;
  }

  static bool decode(const Node& node, msc::BoxFilter& rhs)
  {
    if(!node.IsMap() || node.size() != 1)
      return false;
    
    return true;
  }
};

YAML_NAMESPACE_END

#endif
