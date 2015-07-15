#ifndef _SCENE_H_
#define _SCENE_H_

#include <core/Common.h>
#include <core/EmbreeWrapper.h>

MSC_NAMESPACE_BEGIN

struct Scene
{
  RTCScene rtc_scene;
  // Lights, things and materials
};

MSC_NAMESPACE_END

#endif
