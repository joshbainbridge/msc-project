#ifndef _RAYINTERSECT_H_
#define _RAYINTERSECT_H_

#include <tbb/tbb.h>

#include <core/Common.h>
#include <core/Scene.h>
#include <core/RayUncompressed.h>

MSC_NAMESPACE_BEGIN

class RayIntersect
{
public:
  RayIntersect(Scene* _scene, RayUncompressed* _data)
   : m_scene(_scene)
   , m_data(_data)
  {;}

  void operator()(const tbb::blocked_range< size_t >& r) const;

private:
  Scene* m_scene;
  RayUncompressed* m_data;
};

MSC_NAMESPACE_END

#endif