#ifndef _RAYINTERSECT_H_
#define _RAYINTERSECT_H_

#include <tbb/tbb.h>

#include <core/Common.h>
#include <core/EmbreeWrapper.h>
#include <core/Image.h>
#include <core/Scene.h>
#include <core/RayCompressed.h>

MSC_NAMESPACE_BEGIN

template < typename iterator_type > class Range64 {
public:
  Range64(iterator_type _begin, iterator_type _end)
   : m_begin(_begin)
   , m_end(_end)
  {;}

  Range64(Range64& r, tbb::split)
   : m_end(r.m_end)
   , m_begin(split(r))
  {;}

  bool empty() const {return m_begin == m_end;}
  bool is_divisible() const {return (m_end - m_begin) > 64;}

  iterator_type begin() const {return m_begin;}
  iterator_type end() const {return m_end;}
  iterator_type size() const {return (m_end - m_begin);}

  static const bool is_splittable_in_proportion = false;

private:
  iterator_type m_end;
  iterator_type m_begin;

  iterator_type split(Range64& r)
  {
    iterator_type middle = (r.m_begin + r.m_end) / 2;
    r.m_end = middle;
    return middle;
  }
};

class RayIntersect
{
public:
  RayIntersect(Scene* _scene, RayCompressed* _data)
   : m_scene(_scene)
   , m_data(_data)
  {;}

  void operator()(const Range64< size_t >& r) const;

private:
  Scene* m_scene;
  RayCompressed* m_data;
};

MSC_NAMESPACE_END

#endif