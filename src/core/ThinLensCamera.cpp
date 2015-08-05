#include <core/ThinLensCamera.h>

MSC_NAMESPACE_BEGIN

void ThinLensCamera::construct()
{
  m_transform = Affine3f::Identity()
   * Eigen::Translation3f(m_translation)
   * Eigen::AngleAxisf(m_rotation.y() * M_PI_180, Vector3f::UnitY())
   * Eigen::AngleAxisf(m_rotation.x() * M_PI_180, Vector3f::UnitX())
   * Eigen::AngleAxisf(m_rotation.z() * M_PI_180, Vector3f::UnitZ());

  m_normal = (m_transform.linear().inverse().transpose() * Vector3f::UnitX()).normalized();
}

void ThinLensCamera::sample(const int _count, float* _positions, RandomGenerator* _random, RayCompressed* _ouput) const
{
  Vector3f nodal_point = m_translation + (m_normal * m_focal_length);
  float nodal_focal_ratio = m_focal_distance / m_focal_length;

  msc::Vector3fMap mapped_position(NULL);
  msc::Vector3fMap mapped_direction(NULL);
  for(size_t index = 0; index < _count; ++index)
  {
    new (&mapped_position) msc::Vector3fMap((float*) &(_ouput[index].org));
    new (&mapped_direction) msc::Vector3fMap((float*) &(_ouput[index].dir));

    Vector3f film_position = m_transform * Vector3f(0.f, _positions[2 * index + 1], _positions[2 * index + 0]);
    Vector3f focal_point = film_position + (nodal_point - film_position) * nodal_focal_ratio;

    Vector2f aperture_sample = rejectionSampling(_random) * (m_focal_length / m_aperture);
    Vector3f aperture_position = m_transform * Vector3f(0.f, aperture_sample.x(), aperture_sample.y());

    mapped_position = aperture_position + (m_normal * m_focal_length);
    mapped_direction = (focal_point - mapped_position).normalized();
  }
}

Vector2f ThinLensCamera::rejectionSampling(RandomGenerator* _random) const
{
  float x;
  float y;
  Vector2f output;

  while(true)
  {
    x = _random->sample() - 0.5f;
    y = _random->sample() - 0.5f;
    output = Vector2f(x, y);

    if(output.norm() < 0.5f)
      break;
  }

  return output;
}

MSC_NAMESPACE_END