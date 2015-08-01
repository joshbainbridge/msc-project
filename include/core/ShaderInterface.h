#ifndef _SHADERINTERFACE_H_
#define _SHADERINTERFACE_H_

#include <core/Common.h>
#include <core/OpenImageWrapper.h>

MSC_NAMESPACE_BEGIN

/**
 * @brief      Abstract interface class for surface shaders
 */
class ShaderInterface
{
public:
  virtual ~ShaderInterface() {}

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
  virtual void evaluate(
    const int _size,
    TextureSystem _texture_system,
    const Vector3f* _input,
    const Vector3f* _output,
    const Vector3f* _normal,
    float* _u,
    float* _v,
    float* _result
    ) const =0;

  // virtual void sample(
  //   const int _size,
  //   TextureSystem _texture_system,
  //   const Vector3f* _input,
  //   const Vector3f* _output,
  //   const Vector3f* _normal,
  //   float* _u,
  //   float* _v,
  //   float* _result
  //   ) const =0;
};

MSC_NAMESPACE_END

#endif