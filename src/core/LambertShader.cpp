#include <core/LambertShader.h>

MSC_NAMESPACE_BEGIN

ShaderInterface* LambertShader::clone()
{
  LambertShader* shader = new LambertShader(*this);
  shader->texture(m_texture->clone());
  return shader;
}

void LambertShader::initialize(
  const size_t _size,
  std::vector< float >& _u,
  std::vector< float >& _v,
  TextureSystem _texture_system
  )
{
  m_texture->initialize(_size, _u, _v, _texture_system);
}

float LambertShader::continuation() const
{
  return m_reflectance;
}

void LambertShader::evaluate(
  const size_t _colour_index,
  const Vector3f& _input,
  const Vector3f& _output,
  const Vector3f& _normal,
  Colour3f* _weight,
  float* _cos_theta,
  float* _direct_pdfw
  ) const
{
  float cos_theta_input = _normal.dot(_input);
  float cos_theta_output = _normal.dot(_output);

  if(cos_theta_input < 0.f || cos_theta_output < 0.f)
    *_weight = Colour3f(0.f, 0.f, 0.f);
  else
    *_weight = m_texture->colour(_colour_index) * m_reflectance * M_INV_PI;

  *_cos_theta = (cos_theta_input < 0.f) ? 0.f : cos_theta_input;
  *_direct_pdfw = *_cos_theta * M_INV_PI;
}

void LambertShader::sample(
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
  float x, y, z;
  float azimuth = 2 * M_PI * _random->sample();
  float zenith = acos(sqrt(_random->sample()));
  sphericalToCartesianCoord(azimuth, zenith, &x, &y, &z);
  
  Vector3f temp = (_normal.x() > 0.99f) ? Vector3f(0.f, 1.f, 0.f) : Vector3f(1.f, 0.f, 0.f);
  Vector3f s = temp.cross(_normal).normalized();
  Vector3f t = _normal.cross(s).normalized();

  *_input = x * t + y * s + z * _normal;
  *_weight = m_texture->colour(_colour_index) * m_reflectance * M_INV_PI;
  *_cos_theta = _normal.dot(*_input);
  *_direct_pdfw = *_cos_theta * M_INV_PI;
}

MSC_NAMESPACE_END