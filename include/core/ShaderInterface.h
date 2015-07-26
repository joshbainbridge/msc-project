#ifndef _SHADERINTERFACE_H_
#define _SHADERINTERFACE_H_

#include <core/Common.h>
#include <core/OpenImageWrapper.h>

MSC_NAMESPACE_BEGIN

class ShaderInterface
{
public:
  virtual ~ShaderInterface() {}

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
};

MSC_NAMESPACE_END

#endif