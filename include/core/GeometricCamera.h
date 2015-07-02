#ifndef _GEOMETRICCAMERA_H_
#define _GEOMETRICCAMERA_H_

#include <core/Common.h>
#include <core/CameraInterface.h>

MSC_NAMESPACE_BEGIN

class GeometricCamera : public CameraInterface
{
public:
  inline float aperture() const {return m_aperture;}
  inline float focalDistance() const {return m_focal_distance;}

  inline void aperture(const float _aperture){m_aperture = _aperture;}
  inline void focalDistance(const float _focal_distance){m_focal_distance = _focal_distance;}

  void makeSample(const float xpos, const float ypos, const float ratio, Vector3f *ray_origin, Vector3f *ray_direction, RandomGenerator &_random_generator) const;

private:
  float m_aperture;
  float m_focal_distance;

  Vector2f rejection_sample_circle(RandomGenerator &_random_generator) const;
};

MSC_NAMESPACE_END

YAML_NAMESPACE_BEGIN

template<> struct convert<msc::GeometricCamera>
{
  static Node encode(const msc::GeometricCamera& rhs)
  {
    Node node;
    node["camera"]["type"] = "Geometric";
    node["camera"]["origin"] = rhs.origin();
    node["camera"]["direction"] = rhs.direction();
    node["camera"]["focal length"] = rhs.focalLength();
    node["camera"]["focal distance"] = rhs.focalDistance();
    node["camera"]["aperture"] = rhs.aperture();
    return node;
  }

  static bool decode(const Node& node, msc::GeometricCamera& rhs)
  {
    if(!node.IsMap() || node.size() != 6)
    {
      return false;
    }

    rhs.origin(node["origin"].as<msc::Vector3f>());
    rhs.direction(node["direction"].as<msc::Vector3f>());
    rhs.focalLength(node["focal length"].as<float>());
    rhs.focalDistance(node["focal distance"].as<float>());
    rhs.aperture(node["aperture"].as<float>());
    return true;
  }
};

YAML_NAMESPACE_END

#endif
