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
  LambertShader()
    : m_constant(Colour3f(0.f, 0.f, 0.f))
  {;}

  /**
   * @brief      Getter method for colour coefficient
   *
   * @return     colour coefficient
   */
  inline Colour3f colour() const {return m_constant;}

  /**
   * @brief      Getter method for texture path
   *
   * @return     texture path as string
   */
  inline std::string texture() const {return m_texture_string.string();}

  /**
   * @brief      Setter method for colour coefficient
   *
   * @param[in]  _colour  colour coefficient
   */
  inline void colour(const Colour3f _colour){m_constant = _colour;}

  /**
   * @brief      Setter method for texture path
   *
   * @param[in]  _colour  texture path as string
   */
  inline void texture(const std::string _texture){m_texture_string = OpenImageIO::ustring(_texture);}

  /**
   * @brief      Create polymorphic copy of derived class
   *
   * @return     Is safe to have a covariant return type here
   */
  ShaderInterface* clone();

  /**
   * @brief      Initialize colour values potentially as vectorized texture lookup
   */
  void initialize(
    const size_t _size,
    std::vector< float >& _u,
    std::vector< float >& _v,
    TextureSystem _texture_system
    );

  /**
   * @brief      Evaluate shader for given input and output directions and differential data
   *
   * @param[in]  _colour_index  coeffitient look up index
   * @param[in]  _input         input light direction
   * @param[in]  _output        output light direction
   * @param[in]  _normal        surface normal
   * @param      _weight        resulting bsdf weight
   * @param      _cos_theta     cosine of the angle from input direction
   * @param      _direct_pdfw   probabilty in respect to solid angle
   */
  void evaluate(
    const size_t _colour_index,
    const Vector3f& _input,
    const Vector3f& _output,
    const Vector3f& _normal,
    Colour3f* _weight,
    float* _cos_theta,
    float* _direct_pdfw = NULL
    ) const;

  /**
   * @brief      Create sample according to pdf and produce bsdf weight
   *
   * @param      _random        thread local random generator to prevent mutation
   * @param[in]  _colour_index  index into coefitient look up
   * @param[in]  _output        output light direction
   * @param[in]  _normal        surface normal
   * @param      _input         inpute direction to be created according to pdf
   * @param      _weight        bsdf weight
   * @param      _cos_theta     cosine of the angle between normal and sampled direction
   * @param      _direct_pdfw   probability in respect to solid angle
   */
  void sample(
    RandomGenerator* _random,
    const size_t _colour_index,
    const Vector3f& _output,
    const Vector3f& _normal,
    Vector3f* _input,
    Colour3f* _weight,
    float* _cos_theta,
    float* _direct_pdfw = NULL
    ) const;

private:
  OpenImageIO::ustring m_texture_string;
  Colour3f m_constant;

  std::vector< Colour3f > m_colour;
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