#ifndef _THINLENSCAMERA_H_
#define _THINLENSCAMERA_H_

#include <core/Common.h>
#include <core/CameraInterface.h>

MSC_NAMESPACE_BEGIN

class ThinLensCamera : public CameraInterface
{
public:
  ThinLensCamera()
    : m_translation(Vector3f(0.f, 0.f, 0.f))
    , m_rotation(Vector3f(0.f, 0.f, 0.f))
    , m_focal_length(55.f)
    , m_aperture(5.6f)
    , m_focal_distance(1000.f)
  {;}

  inline Vector3f translation() const {return m_translation;}
  inline Vector3f rotation() const {return m_rotation;}
  inline float focalLength() const {return m_focal_length;}
  inline float aperture() const {return m_aperture;}
  inline float focalDistance() const {return m_focal_distance;}

  void translation(const Vector3f &_translation){m_translation = _translation;}
  void rotation(const Vector3f &_rotation){m_rotation = _rotation;}
  void focalLength(const float _focal_length){m_focal_length = _focal_length;}
  void aperture(const float _aperture){m_aperture = _aperture;}
  void focalDistance(const float _focal_distance){m_focal_distance = _focal_distance;}

  void sample(const int _count, float* _positions, RandomGenerator* _random, RayCompressed* _ouput) const;

private:
  Vector3f m_translation;
  Vector3f m_rotation;
  float m_focal_length;
  float m_aperture;
  float m_focal_distance;

  Vector2f rejectionSampling(RandomGenerator* _random) const;
};

MSC_NAMESPACE_END

YAML_NAMESPACE_BEGIN

template<> struct convert<msc::ThinLensCamera>
{
  static Node encode(const msc::ThinLensCamera& rhs)
  {
    Node node;
    node["camera"]["type"] = "ThinLens";
    node["camera"]["translation"] = rhs.translation();
    node["camera"]["rotation"] = rhs.rotation();
    node["camera"]["focal length"] = rhs.focalLength();
    node["camera"]["focal distance"] = rhs.focalDistance();
    node["camera"]["aperture"] = rhs.aperture();
    return node;
  }

  static bool decode(const Node& node, msc::ThinLensCamera& rhs)
  {
    if(!node.IsMap() || node.size() != 6)
      return false;

    rhs.translation(node["translation"].as<msc::Vector3f>());
    rhs.rotation(node["rotation"].as<msc::Vector3f>());
    rhs.focalLength(node["focal length"].as<float>());
    rhs.focalDistance(node["focal distance"].as<float>());
    rhs.aperture(node["aperture"].as<float>());
    return true;
  }
};

YAML_NAMESPACE_END

#endif
