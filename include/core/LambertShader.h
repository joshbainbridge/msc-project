#ifndef _LAMBERTSHADER_H_
#define _LAMBERTSHADER_H_

#include <string>

#include <core/Common.h>
#include <core/ShaderInterface.h>

MSC_NAMESPACE_BEGIN

/**
 * @brief      Inherits from the shader interface and represents a lambertian surface
 */
class LambertShader : public ShaderInterface
{
public:
  /**
   * @brief      Evaluate shader for given input and output directions and differential data
   *
   * @param[in]  _size            amount of positions to be evaluated
   * @param[in]  _texture_system  thread local texture system for cached data
   * @param[in]  _input           input light direction
   * @param[in]  _output          output light direction
   * @param[in]  _normal          surface normal
   * @param      _u               surface u coordinate for texture lookup
   * @param      _v               surface v coordinate for texture lookup
   * @param      _result          resulting weight of evaluation
   */
  void evaluate(
    const int _size,
    TextureSystem _texture_system,
    const Vector3f* _input,
    const Vector3f* _output,
    const Vector3f* _normal,
    float* _u,
    float* _v,
    float* _result
    ) const;

  /**
   * @brief      Getter method for colour coefficient
   *
   * @return     colour coefficient
   */
  inline Colour3f colour() const {return m_colour;}

  /**
   * @brief      Getter method for texture path
   *
   * @return     texture path as string
   */
  inline std::string texture() const {return m_texture.string();}

  /**
   * @brief      Setter method for colour coefficient
   *
   * @param[in]  _colour  colour coefficient
   */
  inline void colour(const Colour3f _colour){m_colour = _colour;}

  /**
   * @brief      Setter method for texture path
   *
   * @param[in]  _colour  texture path as string
   */
  inline void texture(const std::string _texture){m_texture = OpenImageIO::ustring(_texture);}

private:
  Colour3f m_colour;
  OpenImageIO::ustring m_texture;
};

MSC_NAMESPACE_END

YAML_NAMESPACE_BEGIN

template<> struct convert<msc::LambertShader>
{
  static Node encode(const msc::LambertShader& rhs)
  {
    Node node;
    node["shader"]["type"] = "Lambert";
    node["shader"]["colour"] = rhs.colour();
    node["shader"]["texture"] = rhs.texture();
    return node;
  }

  static bool decode(const Node& node, msc::LambertShader& rhs)
  {
    if(!node.IsMap() || node.size() != 3)
      return false;

    rhs.colour(node["colour"].as<msc::Colour3f>());
    rhs.texture(node["texture"].as<std::string>());
    return true;
  }
};

YAML_NAMESPACE_END

#endif