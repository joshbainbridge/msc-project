#include <core/QuadLight.h>

MSC_NAMESPACE_BEGIN

void QuadLight::construct()
{
  const float tmp_positions[] = {
    0.f, -0.5f, -0.5f, 0.f,
    0.f, 0.5f, -0.5f, 0.f,
    0.f, 0.5f, 0.5f, 0.f,
    0.f, 0.5f, 0.5f, 0.f,
    0.f, -0.5f, 0.5f, 0.f,
    0.f, -0.5f, -0.5f, 0.f
  };

  const unsigned int tmp_indices[] = {
    0, 1, 2,
    3, 4, 5,
  };

  m_positions = std::vector<float>(tmp_positions, tmp_positions + 24);
  m_indices = std::vector<unsigned int>(tmp_indices, tmp_indices + 6);

  m_transform = Affine3f::Identity()
   * Eigen::Translation3f(m_translation)
   * Eigen::AngleAxisf(m_rotation.x() * M_PI_180, Vector3f::UnitX())
   * Eigen::AngleAxisf(m_rotation.y() * M_PI_180, Vector3f::UnitY())
   * Eigen::AngleAxisf(m_rotation.z() * M_PI_180, Vector3f::UnitZ())
   * Eigen::AlignedScaling3f(Vector3f(1.f, m_scale.x(), m_scale.y()));

  m_normal = (m_transform.linear().inverse().transpose() * Vector3f::UnitX()).normalized();

  Vector3fMap mapped_data(NULL);
  for(size_t i = 0; i < 6; ++i)
  {
    new (&mapped_data) Vector3fMap((float*) &(m_positions[4 * i + 0]));
    mapped_data = m_transform * mapped_data;
  }
}

void QuadLight::illuminate(
  RandomGenerator* _random,
  const Vector3f& _position,
  Vector3f* _direction,
  float* _distance,
  Colour3f* _radiance,
  float* _direct_pdfw
  ) const
{
  float sample_x = 0.f;
  float sample_y = _random->sample() - 0.5f;
  float sample_z = _random->sample() - 0.5f;
  Vector3f sample = m_transform * Vector3f(sample_x, sample_y, sample_z);

  float cos_theta = (_position - sample).normalized().dot(m_normal);

  if(cos_theta < 0.f)
    *_radiance = Colour3f(0.f, 0.f, 0.f);
  else
    *_radiance = Colour3f(m_intensity, m_intensity, m_intensity);

  cos_theta = (cos_theta < 0.f) ? 0.f : cos_theta;

  *_direction = (sample - _position).normalized();
  *_distance = (sample - _position).norm();
  *_direct_pdfw = ((sample - _position).squaredNorm() / cos_theta) / (m_scale.x() * m_scale.y());
}

void QuadLight::radiance(
  const Vector3f& _direction,
  Colour3f* _radiance,
  float* _cos_theta,
  float* _direct_pdfa
  ) const
{
  *_cos_theta = m_normal.dot(_direction * -1.f);

  if(*_cos_theta < 0.f)
    *_radiance = Colour3f(0.f, 0.f, 0.f);
  else
    *_radiance = Colour3f(m_intensity, m_intensity, m_intensity);

  *_direct_pdfa = 1.f / (m_scale.x() * m_scale.y());
}

MSC_NAMESPACE_END