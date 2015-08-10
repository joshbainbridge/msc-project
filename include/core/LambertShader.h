#ifndef _LAMBERTSHADER_H_
#define _LAMBERTSHADER_H_

#include <string>

#include <boost/shared_ptr.hpp>

#include <core/Common.h>
#include <core/ShaderInterface.h>
#include <core/TextureInterface.h>
#include <core/StandardTexture.h>
#include <core/ConstantTexture.h>
#include <core/LayeredTexture.h>

MSC_NAMESPACE_BEGIN

/**
 * @brief      Inherits from the shader interface and represents a lambertian surface
 * 
 * This shader will evaluate a perfectly diffuse surface and importance sample the hemisphere
 * according to the cosine term in the rendering equation. This is required for multiple importance
 * sampling within the integrator. It also implements the clone and initialize methods so that
 * texture information can be cached with multiple shading points simultaneously.
 */
class LambertShader : public ShaderInterface
{
public:
  /**
   * @brief      Initialiser list for class
   */
  LambertShader()
    : m_reflectance(1.f)
  {;}

  /**
   * @brief      Getter method for colour coefficient
   *
   * @return     colour coefficient
   */
  inline float reflectance() const {return m_reflectance;}

  /**
   * @brief      Getter method for texture path
   *
   * @return     texture path as string
   */
  inline TextureInterface texture() const {return *m_texture;}

  /**
   * @brief      Setter method for colour coefficient
   *
   * @param[in]  _colour  colour coefficient
   */
  inline void reflectance(const float _reflectance){m_reflectance = _reflectance;}

  /**
   * @brief      Setter method for texture path
   *
   * @param[in]  _colour  texture path as string
   */
  inline void texture(TextureInterface* _texture){m_texture.reset(_texture);}

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
   * @brief      Get probabilty of bsdf reflectance for russian roulette
   *
   * @return     reflectance probabilty
   */
  float continuation() const;

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
  boost::shared_ptr< TextureInterface > m_texture;
  float m_reflectance;
};

MSC_NAMESPACE_END

YAML_NAMESPACE_BEGIN

template<> struct convert<msc::LambertShader>
{
  static bool decode(const Node& node, msc::LambertShader& rhs)
  {
    if(!node.IsMap() || node.size() != 3)
      return false;

    {
      if(node["texture"]["type"].as< std::string >() == "Standard")
      {
        msc::StandardTexture* standard_texture = new msc::StandardTexture();
        *standard_texture = node["texture"].as<msc::StandardTexture>();
        rhs.texture(standard_texture);
      }

      if(node["texture"]["type"].as< std::string >() == "Constant")
      {
        msc::ConstantTexture* constant_texture = new msc::ConstantTexture();
        *constant_texture = node["texture"].as<msc::ConstantTexture>();
        rhs.texture(constant_texture);
      }

      if(node["texture"]["type"].as< std::string >() == "Layered")
      {
        msc::LayeredTexture* layered_texture = new msc::LayeredTexture();
        *layered_texture = node["texture"].as<msc::LayeredTexture>();
        rhs.texture(layered_texture);
      }
    }

    rhs.reflectance(node["reflectance"].as<float>());

    return true;
  }
};

YAML_NAMESPACE_END

#endif