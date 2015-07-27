#include <core/PinHoleCamera.h>

MSC_NAMESPACE_BEGIN

void PinHoleCamera::sample(const int _count, float* _positions, RandomGenerator* _random, RayCompressed* _ouput) const
{
  Affine3f rotate_transform = Affine3f::Identity()
   * Eigen::AngleAxisf(m_rotation.y() * M_PI_180, Vector3f::UnitY())
   * Eigen::AngleAxisf(m_rotation.x() * M_PI_180, Vector3f::UnitX())
   * Eigen::AngleAxisf(m_rotation.z() * M_PI_180, Vector3f::UnitZ());

  Vector3f forward = rotate_transform * Vector3f::UnitX();
  Vector3f up = rotate_transform * Vector3f::UnitY();
  Vector3f right = rotate_transform * Vector3f::UnitZ();

  Vector3f nodal_point = m_translation + (forward * m_focal_length);

  msc::Vector3fMap mapped_position(NULL);
  msc::Vector3fMap mapped_direction(NULL);
  for(size_t iterator = 0; iterator < _count; ++iterator)
  {
    new (&mapped_position) msc::Vector3fMap((float*) &(_ouput[iterator].org));
    new (&mapped_direction) msc::Vector3fMap((float*) &(_ouput[iterator].dir));

    Vector3f film_position = m_translation + (right * _positions[2 * iterator + 0]) + (up * _positions[2 * iterator + 1]);

    mapped_position = nodal_point;
    mapped_direction = (nodal_point - film_position).normalized();
  }
}

MSC_NAMESPACE_END