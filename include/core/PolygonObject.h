#ifndef _POLYGONOBJECT_H_
#define _POLYGONOBJECT_H_

#include <string>
#include <vector>
#include <iostream>

#include <tinyobjloader/tiny_obj_loader.h>

#include <core/Common.h>
#include <core/ObjectInterface.h>
#include <core/Singleton.h>

MSC_NAMESPACE_BEGIN

/**
 * @brief      Inherits from the object interface and represents polygon geometry
 */
class PolygonObject : public ObjectInterface
{
public:
  PolygonObject()
    : m_translation(Vector3f(0.f, 0.f, 0.f))
    , m_rotation(Vector3f(0.f, 0.f, 0.f))
    , m_scale(Vector3f(1.f, 1.f, 1.f))
  {;}

  /**
   * @brief      Getter method for filename
   *
   * @return     filename string
   */
  inline std::string filename() const {return m_filename;}

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
   * @brief      Getter method for scale
   *
   * @return     scale vector
   */
  inline Vector3f scale() const {return m_scale;}

  /**
   * @brief      Getter method for shader id
   *
   * @return     shader id
   */
  inline int shader() const {return m_shader;}

  /**
   * @brief      Get reference to geometry positions
   *
   * @return     positions reference
   */
  inline std::vector<float>& positions() {return m_positions;}

  /**
   * @brief      Get reference to geometry indices
   *
   * @return     indices reference
   */
  inline std::vector<unsigned int>& indices() {return m_indices;}

  /**
   * @brief      Setter method for filename
   *
   * @param[in]  _filename  filename string
   */
  void filename(const std::string &_filename){m_filename = _filename;}

  /**
   * @brief      Setter method for translation
   *
   * @param[in]  _translation  translation vector
   */
  void translation(const Vector3f &_translation){m_translation = _translation;}

  /**
   * @brief      Setter method for rotation
   *
   * @param[in]  _rotation  euler angle vector
   */
  void rotation(const Vector3f &_rotation){m_rotation = _rotation;}

  /**
   * @brief      Setter method for scale
   *
   * @param[in]  _scale  scale vector
   */
  void scale(const Vector3f &_scale){m_scale = _scale;}

  /**
   * @brief      Setter method for shader id
   *
   * @param[in]  _shader  shader id
   */
  void shader(const int &_shader){m_shader = _shader;}

  /**
   * @brief      Precomputes transform and geometry information
   */
  void construct();

  /**
   * @brief      Gets u and v texture coordinates
   *
   * @param[in]  _primitive  primitive index value
   * @param[in]  _s          s coordinate
   * @param[in]  _t          t coordinate
   * @param      _output     output uv coordinates
   */
  void texture(
    const size_t _primitive,
    const float _s,
    const float _t,
    Vector2f* _output
    ) const;

  /**
   * @brief      Gets normal direction
   *
   * @param[in]  _primitive  primitive index value
   * @param[in]  _s          s coordinate
   * @param[in]  _t          t coordinate
   * @param      _output     output uv coordinates
   */
  void normal(
    const size_t _primitive,
    const float _s,
    const float _t,
    Vector3f* _output
    ) const;

private:
  std::string m_filename;
  Vector3f m_translation;
  Vector3f m_rotation;
  Vector3f m_scale;
  int m_shader;

  std::vector< float > m_positions;
  std::vector< float > m_normals;
  std::vector< float > m_texcoords;
  std::vector< unsigned int > m_indices;
};

MSC_NAMESPACE_END

YAML_NAMESPACE_BEGIN

template<> struct convert<msc::PolygonObject>
{
  static Node encode(const msc::PolygonObject& rhs)
  {
    Node node;
    node["object"]["filename"] = rhs.filename();
    node["object"]["translation"] = rhs.translation();
    node["object"]["rotation"] = rhs.rotation();
    node["object"]["scale"] = rhs.scale();
    node["object"]["shader"] = rhs.shader();
    return node;
  }

  static bool decode(const Node& node, msc::PolygonObject& rhs)
  {
    if(!node.IsMap() || node.size() != 6)
      return false;

    msc::SingletonString& scene_root = msc::SingletonString::instance();
    rhs.filename(scene_root.getData().append("/").append(node["filename"].as<std::string>()));
    rhs.translation(node["translation"].as<msc::Vector3f>());
    rhs.rotation(node["rotation"].as<msc::Vector3f>());
    rhs.scale(node["scale"].as<msc::Vector3f>());
    rhs.shader(node["shader"].as<int>());

    rhs.construct();

    return true;
  }
};

YAML_NAMESPACE_END

#endif
