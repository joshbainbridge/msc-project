#ifndef _RAYBOUNDINGBOX_H_
#define _RAYBOUNDINGBOX_H_

#include <tbb/tbb.h>

#include <core/Common.h>
#include <core/RayUncompressed.h>

MSC_NAMESPACE_BEGIN

class RayBoundingbox
{
public:
  RayBoundingbox(RayUncompressed* _data)
   : m_data(_data)
  {;}

  RayBoundingbox(RayBoundingbox& s, tbb::split )
   : m_data(s.m_data)
  {;}

  void operator()(const tbb::blocked_range< size_t >& r);
  void join( RayBoundingbox& rhs );

  inline BoundingBox3f value() const {return m_value;}

private:
  RayUncompressed* m_data;
  BoundingBox3f m_value;
};

MSC_NAMESPACE_END

#endif