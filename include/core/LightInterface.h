#ifndef _LIGHTINTERFACE_H_
#define _LIGHTINTERFACE_H_

#include <core/Common.h>
#include <core/RayCompressed.h>
#include <core/RandomGenerator.h>

MSC_NAMESPACE_BEGIN

class LightInterface
{
public:
  virtual ~LightInterface() {}

  virtual void sample(const unsigned int _size, RandomGenerator* _random, Vector3f* _position, Vector3f* _input, Colour3f* _radiance) const =0;
};

MSC_NAMESPACE_END

#endif
