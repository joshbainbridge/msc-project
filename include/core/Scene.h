#ifndef _SCENE_H_
#define _SCENE_H_

#include <vector>
#include <map>

#include <boost/shared_ptr.hpp>

#include <core/Common.h>
#include <core/EmbreeWrapper.h>
#include <core/ObjectInterface.h>
#include <core/ShaderInterface.h>
#include <core/LightInterface.h>

MSC_NAMESPACE_BEGIN

/**
 * @brief      Data structure for scene information such as objects, lights and shaders
 * 
 * The scene structure contains std::vectors of polymorphic pointers to implemented objects on
 * the heap. Smart pointers are used to manage memory and there is also a map that represents the
 * relationship between shaders and lights. The RTCScene is the acceleration structure used by
 * Embree to traverse rays across geometry stored in the objects vector. The scene should not mutate
 * after initial construction.
 */
struct Scene
{
  RTCScene rtc_scene;

  std::vector< boost::shared_ptr< ObjectInterface > > objects;
  std::vector< boost::shared_ptr< ShaderInterface > > shaders;
  std::vector< boost::shared_ptr< LightInterface > > lights;
  std::map< int, int > shaders_to_lights;
};

MSC_NAMESPACE_END

#endif
