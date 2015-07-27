#ifndef _PINHOLECAMERA_H_
#define _PINHOLECAMERA_H_

#include <core/Common.h>
#include <core/CameraInterface.h>

MSC_NAMESPACE_BEGIN

class PinHoleCamera : public CameraInterface
{
public:
  PinHoleCamera()
    : m_translation(Vector3f(0.f, 0.f, 0.f))
    , m_rotation(Vector3f(0.f, 0.f, 0.f))
    , m_focal_length(55.f)
  {;}

  inline Vector3f translation() const {return m_translation;}
  inline Vector3f rotation() const {return m_rotation;}
  inline float focalLength() const {return m_focal_length;}

  void translation(const Vector3f &_translation){m_translation = _translation;}
  void rotation(const Vector3f &_rotation){m_rotation = _rotation;}
  void focalLength(const float _focal_length){m_focal_length = _focal_length;}

  void sample(const int _count, float* _positions, RandomGenerator* _random, RayCompressed* _ouput) const;

private:
  Vector3f m_translation;
  Vector3f m_rotation;
  float m_focal_length;
};

MSC_NAMESPACE_END

YAML_NAMESPACE_BEGIN

template<> struct convert<msc::PinHoleCamera>
{
  static Node encode(const msc::PinHoleCamera& rhs)
  {
    Node node;
    node["camera"]["type"] = "PinHole";
    node["camera"]["translation"] = rhs.translation();
    node["camera"]["rotation"] = rhs.rotation();
    node["camera"]["focal length"] = rhs.focalLength();
    return node;
  }

  static bool decode(const Node& node, msc::PinHoleCamera& rhs)
  {
    if(!node.IsMap() || node.size() != 4)
      return false;

    rhs.translation(node["translation"].as<msc::Vector3f>());
    rhs.rotation(node["rotation"].as<msc::Vector3f>());
    rhs.focalLength(node["focal length"].as<float>());
    return true;
  }
};

YAML_NAMESPACE_END

#endif
