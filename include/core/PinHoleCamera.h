#ifndef _PINHOLECAMERA_H_
#define _PINHOLECAMERA_H_

#include <core/Common.h>
#include <core/CameraInterface.h>

MSC_NAMESPACE_BEGIN

/**
 * @brief      Inherits from the camera interface and represents a pin hole camera
 */
class PinHoleCamera : public CameraInterface
{
public:
  PinHoleCamera()
    : m_translation(Vector3f(0.f, 0.f, 0.f))
    , m_rotation(Vector3f(0.f, 0.f, 0.f))
    , m_focal_length(55.f)
  {;}

  /**
   * @brief      Getter method for translation
   *
   * @return     translation vector
   */
  inline Vector3f translation() const {return m_translation;}

  /**
   * @brief      Getter method for rotation
   *
   * @return     euler angle vector
   */
  inline Vector3f rotation() const {return m_rotation;}

  /**
   * @brief      Getter method for focal length
   *
   * @return     focal length
   */
  inline float focalLength() const {return m_focal_length;}

  /**
   * @brief      Setter method for translation
   *
   * @param[in]  _translation  translation vector
   */
  void translation(const Vector3f &_translation){m_translation = _translation;}

  /**
   * @brief      Setter method for rotation
   *
   * @param[in]  _translation  euler angle vector
   */
  void rotation(const Vector3f &_rotation){m_rotation = _rotation;}

  /**
   * @brief      Setter method for focal length
   *
   * @param[in]  _translation  focal length
   */
  void focalLength(const float _focal_length){m_focal_length = _focal_length;}

  /**
   * @brief      Creates primary rays from camera
   *
   * @param[in]  _count      sample count to process
   * @param      _positions  positions on film plane of samples
   * @param      _random     thread local random generator to prevent mutation
   * @param      _ouput      output of compressed rays
   */
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
