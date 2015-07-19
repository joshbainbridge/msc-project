#ifndef _RAYSORT_H_
#define _RAYSORT_H_

#include <tbb/tbb.h>

#include <core/Common.h>
#include <core/RayUncompressed.h>

MSC_NAMESPACE_BEGIN

// class RaySort2
// {
// public:
//   RaySort2(RayUncompressed* _output, BoundingBox3f m_limits)
//    : m_output(_output)
//    , m_limits(_limits)
//   {;}

//   RaySort2(RaySort2& s, tbb::split )
//    : m_data(s.m_data)
//   {;}

//   void operator()(const tbb::blocked_range< size_t >& r);
//   void join( RaySort2& rhs );

//   inline BoundingBox3f value() const {return m_value;}

// private:
//   RayUncompressed* m_output;
//   BoundingBox3f m_limits;
// };

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