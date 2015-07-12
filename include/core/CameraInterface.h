#ifndef _CAMERAINTERFACE_H_
#define _CAMERAINTERFACE_H_

#include <core/Common.h>
#include <core/Ray.h>
#include <core/RandomGenerator.h>

MSC_NAMESPACE_BEGIN

class CameraInterface
{
public:
  virtual ~CameraInterface() {}

  inline Vector3f direction() const {return m_dir;}
  inline Vector3f origin() const {return m_origin;}
  inline Vector3f up() const {return m_up;}
  inline Vector3f right() const {return m_right;}
  inline float focalLength() const {return m_focal_length;}

  void direction(const Vector3f &_dir);
  void origin(const Vector3f &_origin){m_origin = _origin;}
  void focalLength(const float _focal_length){m_focal_length = _focal_length;}

  virtual void sample(const int _count, float* _positions, RandomGenerator* _random, Ray* _ouput) const =0;

protected:
  //Protected variables
  Vector3f m_origin;
  Vector3f m_dir;
  Vector3f m_up;
  Vector3f m_right;
  float m_focal_length;
};

MSC_NAMESPACE_END

#endif
