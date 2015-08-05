#include <core/PinHoleCamera.h>

MSC_NAMESPACE_BEGIN

void PinHoleCamera::construct()
{
  m_transform = Affine3f::Identity()
   * Eigen::Translation3f(m_translation)
   * Eigen::AngleAxisf(m_rotation.y() * M_PI_180, Vector3f::UnitY())
   * Eigen::AngleAxisf(m_rotation.x() * M_PI_180, Vector3f::UnitX())
   * Eigen::AngleAxisf(m_rotation.z() * M_PI_180, Vector3f::UnitZ());

  m_normal = (m_transform.linear().inverse().transpose() * Vector3f::UnitX()).normalized();
}

void PinHoleCamera::sample(const int _count, float* _positions, RandomGenerator* _random, RayCompressed* _ouput) const
{
  Vector3f nodal_point = m_translation + (m_normal * m_focal_length);

  msc::Vector3fMap mapped_position(NULL);
  msc::Vector3fMap mapped_direction(NULL);
  for(size_t index = 0; index < _count; ++index)
  {
    new (&mapped_position) msc::Vector3fMap((float*) &(_ouput[index].org));
    new (&mapped_direction) msc::Vector3fMap((float*) &(_ouput[index].dir));

    Vector3f film_position = m_transform * Vector3f(0.f, _positions[2 * index + 1],  _positions[2 * index + 0]);

    mapped_position = nodal_point;
    mapped_direction = (nodal_point - film_position).normalized();
  }
}

MSC_NAMESPACE_END