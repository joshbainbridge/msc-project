#ifndef _RAYSORT_H_
#define _RAYSORT_H_

#include <tbb/tbb.h>

#include <core/Common.h>
#include <core/RayUncompressed.h>

MSC_NAMESPACE_BEGIN

/**
 * @brief      Sorts rays according to a ray position and direction recursively and in parallel
 */
class RaySort
{
public:
  RaySort(
    size_t _begin,
    size_t _end,
    BoundingBox3f _limits,
    RayUncompressed* _output
    )
   : m_begin(_begin)
   , m_end(_end)
   , m_limits(_limits)
   , m_output(_output)
  {;}

  /**
   * @brief      Operator overloader to allow the class to act as a functor with tbb
   */
  void operator()() const;

private:
  size_t m_begin;
  size_t m_end;
  BoundingBox3f m_limits;
  RayUncompressed* m_output;
};

MSC_NAMESPACE_END

#endif