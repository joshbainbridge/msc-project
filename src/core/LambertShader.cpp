#include <core/LambertShader.h>

MSC_NAMESPACE_BEGIN

void LambertShader::evaluate(
  const Vector3f _input,
  const Vector3f _output,
  const Vector3f _normal,
  OpenImageIO::TextureSystem* _texture_system,
  const float _u,
  const float _v,
  Colour3f* _weight
  ) const
{
  float result[3] = {1.f, 1.f, 1.f};
  OpenImageIO::TextureOpt options;

  _texture_system->texture(m_texture, options, _u, _v, 0, 0, 0, 0, 3, result);

  *_weight = m_colour * Colour3f(result[0], result[1], result[2]);
}

MSC_NAMESPACE_END