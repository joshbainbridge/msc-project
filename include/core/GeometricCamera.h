#ifndef _GEOMETRICCAMERA_H_
#define _GEOMETRICCAMERA_H_

#include <core/Common.h>
#include <core/CameraInterface.h>

MSC_NAMESPACE_BEGIN

class GeometricCamera : public CameraInterface
{
public:
  GeometricCamera()
    : m_origin(Vector3f(0.f, 0.f, 0.f))
    , m_dir(Vector3f(1.f, 0.f, 0.f))
    , m_up(Vector3f(0.f, 1.f, 0.f))
    , m_right(Vector3f(0.f, 0.f, 1.f))
    , m_focal_length(55.f)
    , m_aperture(5.6f)
    , m_focal_distance(1000.f)
  {;}

  inline Vector3f direction() const {return m_dir;}
  inline Vector3f origin() const {return m_origin;}
  inline Vector3f up() const {return m_up;}
  inline Vector3f right() const {return m_right;}
  inline float focalLength() const {return m_focal_length;}

  void direction(const Vector3f &_dir);
  void origin(const Vector3f &_origin){m_origin = _origin;}
  void focalLength(const float _focal_length){m_focal_length = _focal_length;}

  inline float aperture() const {return m_aperture;}
  inline float focalDistance() const {return m_focal_distance;}

  inline void aperture(const float _aperture){m_aperture = _aperture;}
  inline void focalDistance(const float _focal_distance){m_focal_distance = _focal_distance;}

  void sample(const int _count, float* _positions, RandomGenerator* _random, RayCompressed* _ouput) const;

private:
  Vector3f m_origin;
  Vector3f m_dir;
  Vector3f m_up;
  Vector3f m_right;
  float m_focal_length;
  float m_aperture;
  float m_focal_distance;

  Vector2f rejectionSampling(RandomGenerator* _random) const;
};

MSC_NAMESPACE_END

YAML_NAMESPACE_BEGIN

template<> struct convert<msc::GeometricCamera>
{
  static Node encode(const msc::GeometricCamera& rhs)
  {
    Node node;
    node["camera"]["type"] = "Geometric";
    node["camera"]["translation"] = rhs.origin();
    node["camera"]["rotation"] = rhs.direction();
    node["camera"]["focal length"] = rhs.focalLength();
    node["camera"]["focal distance"] = rhs.focalDistance();
    node["camera"]["aperture"] = rhs.aperture();
    return node;
  }

  static bool decode(const Node& node, msc::GeometricCamera& rhs)
  {
    if(!node.IsMap() || node.size() != 6)
      return false;

    rhs.origin(node["translation"].as<msc::Vector3f>());
    rhs.direction(node["rotation"].as<msc::Vector3f>());
    rhs.focalLength(node["focal length"].as<float>());
    rhs.focalDistance(node["focal distance"].as<float>());
    rhs.aperture(node["aperture"].as<float>());
    return true;
  }
};

YAML_NAMESPACE_END

#endif
