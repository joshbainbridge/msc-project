#include <core/NullShader.h>

MSC_NAMESPACE_BEGIN

ShaderInterface* NullShader::clone()
{
  return new NullShader(*this);
}

void NullShader::initialize(
  const size_t _size,
  std::vector< float >& _u,
  std::vector< float >& _v,
  TextureSystem _texture_system
  )
{
  // Nothing to initialize
}

float NullShader::continuation() const
{
  return 1.f;
}

void NullShader::evaluate(
  const size_t _colour_index,
  const Vector3f& _input,
  const Vector3f& _output,
  const Vector3f& _normal,
  Colour3f* _weight,
  float* _cos_theta,
  float* _direct_pdfw
  ) const
{
  *_weight = Colour3f(0.f, 0.f, 0.f);
  *_cos_theta = 1.f;
  *_direct_pdfw = 1.f * M_INV_PI * 0.5f;
}

void NullShader::sample(
  RandomGenerator* _random,
  const size_t _colour_index,
  const Vector3f& _output,
  const Vector3f& _normal,
  Vector3f* _input,
  Colour3f* _weight,
  float* _cos_theta,
  float* _direct_pdfw
  ) const
{
  *_weight = Colour3f(0.f, 0.f, 0.f);
  *_cos_theta = 1.f;
  *_direct_pdfw = 1.f * M_INV_PI * 0.5f;
}

MSC_NAMESPACE_END