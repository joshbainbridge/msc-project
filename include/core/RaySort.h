#ifndef _RAYSORT_H_
#define _RAYSORT_H_

#include <tbb/tbb.h>

#include <core/Common.h>
#include <core/RayUncompressed.h>

MSC_NAMESPACE_BEGIN

class RaySort : public tbb::task
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

  tbb::task* execute();

private:
  size_t m_begin;
  size_t m_end;
  BoundingBox3f m_limits;
  RayUncompressed* m_output;
};

MSC_NAMESPACE_END

#endif