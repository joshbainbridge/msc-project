#ifndef _SCENE_H_
#define _SCENE_H_

#include <map>

#include <core/Common.h>
#include <core/PolygonObject.h>
#include <core/EmbreeWrapper.h>

MSC_NAMESPACE_BEGIN

struct Scene
{
  RTCScene rtc_scene;

  std::map< size_t, PolygonObject > objects;
  // std::vector< LightInterface > lights;
  // std::vector< MaterialInterface > materials;
};

MSC_NAMESPACE_END

#endif
