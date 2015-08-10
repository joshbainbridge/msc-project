#ifndef _NULLSHADER_H_
#define _NULLSHADER_H_

#include <core/Common.h>
#include <core/ShaderInterface.h>

MSC_NAMESPACE_BEGIN

/**
 * @brief      Inherits from the shader interface and represents a non-contributing surface
 * 
 * Represents a shader that does not contribute to the lighting intergral and is primarily used
 * as a simple index toward lights that have geometric representation.
 */
class NullShader : public ShaderInterface
{
public:
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
};

MSC_NAMESPACE_END

YAML_NAMESPACE_BEGIN

template<> struct convert<msc::NullShader>
{
  static bool decode(const Node& node, msc::NullShader& rhs)
  {
    if(!node.IsMap() || node.size() != 1)
      return false;

    return true;
  }
};

YAML_NAMESPACE_END

#endif