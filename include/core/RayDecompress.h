#ifndef _RAYDECOMPRESS_H_
#define _RAYDECOMPRESS_H_

#include <tbb/tbb.h>

#include <core/Common.h>
#include <core/RayCompressed.h>
#include <core/RayUncompressed.h>

MSC_NAMESPACE_BEGIN

/**
 * @brief      Functor class to decompress an array of rays
 * 
 * This class acts as a functor to decompress ray batches so that they can be sorted and traced
 * in a parallel manner using tbb.
 */
class RayDecompress
{
public:
  /**
   * @brief      Initialiser list for class
   */
  RayDecompress(RayCompressed* _input, RayUncompressed* _output)
   : m_input(_input)
   , m_output(_output)
  {;}

  /**
   * @brief      Operator overloader to allow the class to act as a functor with tbb
   * 
   * @param[in]  r           a one dimensional range over an array of rays
   */
  void operator()(const tbb::blocked_range< size_t >& r) const;

private:
  RayCompressed* m_input;
  RayUncompressed* m_output;
};

MSC_NAMESPACE_END

#endif