#include <core/QuadLight.h>

MSC_NAMESPACE_BEGIN

void QuadLight::illuminate(const unsigned int _size, RandomGenerator* _random, Vector3f* _position, Vector3f* _input, Colour3f* _radiance) const
{
  Affine3f transform = Affine3f::Identity()
   * Eigen::AngleAxisf(m_rotation.y() * M_PI_180, Vector3f::UnitY())
   * Eigen::AngleAxisf(m_rotation.x() * M_PI_180, Vector3f::UnitX())
   * Eigen::AngleAxisf(m_rotation.z() * M_PI_180, Vector3f::UnitZ());

  Vector3f forward = transform * Vector3f::UnitX();
  Vector3f up = transform * Eigen::AlignedScaling3f(m_scale) * Vector3f::UnitY();
  Vector3f right = transform * Eigen::AlignedScaling3f(m_scale) * Vector3f::UnitZ();

  for(size_t index = 0; index < _size; ++index)
  {
    float sample_x = _random->getSample();
    float sample_y = _random->getSample();
    Vector3f sample = m_translation + up * sample_x + right * sample_y;

    _input[index] = (sample - _position[index]).normalized();

    float falloff = (1000.f / pow((sample - _position[index]).norm(), 2));
    float cosine = (_position[index] - sample).normalized().dot(forward);
    cosine = (cosine < 0.f) ? 0.f : cosine;

    _radiance[index] = Colour3f(m_radiance, m_radiance, m_radiance) * falloff * cosine;
  }
}

MSC_NAMESPACE_END