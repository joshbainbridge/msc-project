#ifndef _SHADERINTERFACE_H_
#define _SHADERINTERFACE_H_

#include <vector>

#include <core/Common.h>
#include <core/RandomGenerator.h>
#include <core/OpenImageWrapper.h>

MSC_NAMESPACE_BEGIN

/**
 * @brief      Abstract interface class for surface shaders
 * 
 * This is an interface for using a shader in a polymorphic sense. Due to the design of the
 * integrator the shader will take only a single position on the surface to evaluate at a time.
 * It will also be responsible for sampling according to a probability density and returning such
 * data and for use with multiple importance sampling. Access to textures across multiple positions
 * are pre calculated and cached to improve memory performance.
 */
class ShaderInterface
{
public:
  /**
   * @brief      Virtual destructor required for interface
   */
  virtual ~ShaderInterface() {}

  /**
   * @brief      Create polymorphic copy of derived class
   *
   * @return     is safe to have a covariant return type here
   */
  virtual ShaderInterface* clone() = 0;

  /**
   * @brief      Initialize colour values potentially as vectorized texture lookup
   */
  virtual void initialize(
    const size_t _size,
    std::vector< float >& _u,
    std::vector< float >& _v,
    TextureSystem _texture_system
    ) =0;

  /**
   * @brief      Get probabilty of bsdf reflectance for russian roulette
   *
   * @return     reflectance probabilty
   */
  virtual float continuation() const = 0;

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
  virtual void evaluate(
    const size_t _colour_index,
    const Vector3f& _input,
    const Vector3f& _output,
    const Vector3f& _normal,
    Colour3f* _weight,
    float* _cos_theta,
    float* _direct_pdfw = NULL
    ) const =0;

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
  virtual void sample(
    RandomGenerator* _random,
    const size_t _colour_index,
    const Vector3f& _output,
    const Vector3f& _normal,
    Vector3f* _input,
    Colour3f* _weight,
    float* _cos_theta,
    float* _direct_pdfw = NULL
    ) const =0;
};

MSC_NAMESPACE_END

#endif