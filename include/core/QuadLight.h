#ifndef _QUADLIGHT_H_
#define _QUADLIGHT_H_

#include <core/Common.h>
#include <core/LightInterface.h>

MSC_NAMESPACE_BEGIN

class QuadLight : public LightInterface
{
public:
  QuadLight() : LightInterface() {;}

  Colour3f illumination(const Vector3f &light_direction) const;

  Vector3f sampleLight(RandomGenerator &_random_generator) const;
};

MSC_NAMESPACE_END

#endif
