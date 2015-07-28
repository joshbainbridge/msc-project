#ifndef _QUADLIGHT_H_
#define _QUADLIGHT_H_

#include <core/Common.h>
#include <core/LightInterface.h>

MSC_NAMESPACE_BEGIN

class QuadLight : public LightInterface
{
public:
  QuadLight()
    : m_translation(Vector3f(0.f, 0.f, 0.f))
    , m_rotation(Vector3f(0.f, 0.f, 0.f))
    , m_scale(Vector3f(1.f, 1.f, 1.f))
    , m_radiance(100.f)
  {;}

  inline Vector3f translation() const {return m_translation;}
  inline Vector3f rotation() const {return m_rotation;}
  inline Vector3f scale() const {return m_scale;}
  inline float radiance() const {return m_radiance;}

  void translation(const Vector3f &_translation){m_translation = _translation;}
  void rotation(const Vector3f &_rotation){m_rotation = _rotation;}
  void scale(const Vector3f &_scale){m_scale = _scale;}
  void radiance(const float _radiance){m_radiance = _radiance;}

  void sample(const unsigned int _size, RandomGenerator* _random, Vector3f* _position, Vector3f* _input, Colour3f* _radiance) const;

private:
  Vector3f m_translation;
  Vector3f m_rotation;
  Vector3f m_scale;
  float m_radiance;
};

MSC_NAMESPACE_END

YAML_NAMESPACE_BEGIN

template<> struct convert<msc::QuadLight>
{
  static Node encode(const msc::QuadLight& rhs)
  {
    Node node;
    node["light"]["type"] = "Quad";
    node["light"]["translation"] = rhs.translation();
    node["light"]["rotation"] = rhs.rotation();
    node["light"]["scale"] = rhs.scale();
    node["light"]["radiance"] = rhs.radiance();
    return node;
  }

  static bool decode(const Node& node, msc::QuadLight& rhs)
  {
    if(!node.IsMap() || node.size() != 5)
      return false;

    rhs.translation(node["translation"].as<msc::Vector3f>());
    rhs.rotation(node["rotation"].as<msc::Vector3f>());
    rhs.scale(node["scale"].as<msc::Vector3f>());
    rhs.radiance(node["radiance"].as<float>());
    return true;
  }
};

YAML_NAMESPACE_END

#endif
