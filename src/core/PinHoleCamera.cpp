#include <core/PinHoleCamera.h>

MSC_NAMESPACE_BEGIN

void PinHoleCamera::sample(const int _count, float* _positions, RandomGenerator* _random, RayCompressed* _ouput) const
{
  //Construct nodal position
  Vector3f nodal_point = m_origin + (m_dir * m_focal_length);

  for(size_t iterator = 0; iterator < _count; ++iterator)
  {
    //Find position on film plane
    Vector3f film_position = m_origin + (m_right * _positions[2 * iterator + 0]) + (m_up * _positions[2 * iterator + 1]);

    //Given x and y position between -1 and 1 generate ray postioned on camera plane and calculate direction
    Vector3f direction = (nodal_point - film_position).normalized();
    Vector3f position = nodal_point;

    //Update data
    std::copy(position.data(), position.data() + 3, _ouput[iterator].org);
    std::copy(direction.data(), direction.data() + 3, _ouput[iterator].dir);
  }
}

void PinHoleCamera::direction(const Vector3f &_dir)
{
  //Set camera direction and normalize
  m_dir = _dir.normalized();

  //Recalculate right and up vectors
  m_right = Vector3f(0.f, 1.f, 0.f).cross(m_dir).normalized();
  m_up = m_dir.cross(m_right).normalized();
}

MSC_NAMESPACE_END