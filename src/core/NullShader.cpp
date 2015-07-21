#include <core/NullShader.h>

MSC_NAMESPACE_BEGIN

void NullShader::evaluate(
  const Vector3f _input,
  const Vector3f _output,
  const Vector3f _normal,
  OpenImageIO::TextureSystem* _texture_system,
  const float _u,
  const float _v,
  Colour3f* _weight
  ) const
{
  *_weight = Colour3f(0.f, 0.f, 0.f);
}

MSC_NAMESPACE_END