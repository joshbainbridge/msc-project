#ifndef _RAYINTERSECT_H_
#define _RAYINTERSECT_H_

#include <tbb/tbb.h>

#include <core/Common.h>
#include <core/Scene.h>
#include <core/RayUncompressed.h>

MSC_NAMESPACE_BEGIN

/**
 * @brief      Functor class to intersect rays with scene geometry
 * 
 * A simple functor class to allow parallelism while traversing rays across scene geometry using tbb.
 */
class RayIntersect
{
public:
  /**
   * @brief      Initialiser list for class
   */
  RayIntersect(Scene* _scene, RayUncompressed* _data)
   : m_scene(_scene)
   , m_data(_data)
  {;}

  /**
   * @brief      Operator overloader to allow the class to act as a functor with tbb
   * 
   * @param[in]  r           a one dimensional range over an array of rays
   */
  void operator()(const tbb::blocked_range< size_t >& r) const;

private:
  Scene* m_scene;
  RayUncompressed* m_data;
};

MSC_NAMESPACE_END

#endif