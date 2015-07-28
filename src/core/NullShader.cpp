#include <core/NullShader.h>

MSC_NAMESPACE_BEGIN

void NullShader::evaluate(
  const int _size,
  TextureSystem _texture_system,
  const Vector3f* _input,
  const Vector3f* _output,
  const Vector3f* _normal,
  float* _u,
  float* _v,
  float* _result
  ) const
{
  for(size_t i = 0; i < _size; ++i)
  {
    _result[3 * i + 0] = 0.f;
    _result[3 * i + 1] = 0.f;
    _result[3 * i + 2] = 0.f;
  }
}

MSC_NAMESPACE_END