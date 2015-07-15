#ifndef _RAYSORT_H_
#define _RAYSORT_H_

#include <tbb/tbb.h>

#include <core/Common.h>
#include <core/RayCompressed.h>

MSC_NAMESPACE_BEGIN

class RayCompare
{
public:
  RayCompare(int _axis = 0) : m_axis(_axis) {;}

  bool operator()(RayCompressed const &a, RayCompressed const &b)
  {
    return a.org[m_axis] < b.org[m_axis];
  }

private:
  size_t m_axis;
};

class RaySort : public tbb::task
{
public:
  RaySort(size_t _begin, size_t _end, BoundingBox3f _limits, RayCompressed* _array)
   : m_begin(_begin)
   , m_end(_end)
   , m_limits(_limits)
   , m_array(_array)
  {;}

  tbb::task* execute();

private:
  size_t m_begin;
  size_t m_end;

  BoundingBox3f m_limits;
  RayCompressed* m_array;
};

MSC_NAMESPACE_END

#endif