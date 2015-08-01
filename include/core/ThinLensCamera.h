#ifndef _THINLENSCAMERA_H_
#define _THINLENSCAMERA_H_

#include <core/Common.h>
#include <core/CameraInterface.h>

MSC_NAMESPACE_BEGIN

/**
 * @brief      Inherits from the camera interface and represents a lens based camera
 */
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
   * @brief      Getter method for lens aperture
   *
   * @return     aperture
   */
  inline float aperture() const {return m_aperture;}

  /**
   * @brief      Getter method for focal distance
   *
   * @return     focal distance
   */
  inline float focalDistance() const {return m_focal_distance;}

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
   * @brief      Setter method for lens aperture
   *
   * @param[in]  _translation  aperture
   */
  void aperture(const float _aperture){m_aperture = _aperture;}

  /**
   * @brief      Setter method for focal distance
   *
   * @param[in]  _translation  focal distance
   */
  void focalDistance(const float _focal_distance){m_focal_distance = _focal_distance;}

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
