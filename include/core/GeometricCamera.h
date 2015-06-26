#ifndef _GEOMETRICCAMERA_H_
#define _GEOMETRICCAMERA_H_

#include <core/Common.h>
#include <core/CameraInterface.h>
#include <core/RandomGenerator.h>

MSC_NAMESPACE_BEGIN

class GeometricCamera : public CameraInterface
{
public:
  GeometricCamera();

  inline float aperture() const {return m_aperture;}
  inline float focalDistance() const {return m_focal_distance;}

  inline void aperture(const float _aperture){m_aperture = _aperture;}
  inline void focalDistance(const float _focal_distance){m_focal_distance = _focal_distance;}

  void makeSample(const float xpos, const float ypos, Vector3f *ray_origin, Vector3f *ray_direction, RandomGenerator &_random_generator) const;

private:
  float m_aperture;
  float m_focal_distance;

  Vector2f rejection_sample_circle(RandomGenerator &_random_generator) const;
};

MSC_NAMESPACE_END

#endif
