#ifndef _CAMERAINTERFACE_H_
#define _CAMERAINTERFACE_H_

#include <core/Common.h>
#include <core/RayCompressed.h>
#include <core/RandomGenerator.h>

MSC_NAMESPACE_BEGIN

class CameraInterface
{
public:
  virtual ~CameraInterface() {}

  virtual void sample(const int _count, float* _positions, RandomGenerator* _random, RayCompressed* _ouput) const =0;
};

MSC_NAMESPACE_END

#endif
