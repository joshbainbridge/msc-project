#ifndef _RAYDECOMPRESS_H_
#define _RAYDECOMPRESS_H_

#include <tbb/tbb.h>

#include <core/Common.h>
#include <core/RayCompressed.h>
#include <core/RayUncompressed.h>

MSC_NAMESPACE_BEGIN

class RayDecompress
{
public:
  RayDecompress(RayCompressed* _input, RayUncompressed* _output)
   : m_input(_input)
   , m_output(_output)
  {;}

  void operator()(const tbb::blocked_range< size_t >& r) const;

private:
  RayCompressed* m_input;
  RayUncompressed* m_output;
};

MSC_NAMESPACE_END

#endif