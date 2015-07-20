#include <core/GeometricCamera.h>

MSC_NAMESPACE_BEGIN

void GeometricCamera::sample(const int _count, float* _positions, RandomGenerator* _random, RayCompressed* _ouput) const
{
  //Construct nodal position
  Vector3f nodal_point = m_origin + (m_dir * m_focal_length);
  float nodal_focal_ratio = m_focal_distance / m_focal_length;

  for(size_t iterator = 0; iterator < _count; ++iterator)
  {
    //Find position on film plane
    Vector3f film_position = m_origin + (m_right * _positions[2 * iterator + 0]) + (m_up * _positions[2 * iterator + 1]);

    //Find ray intersection on focal plane
    Vector3f focal_point = film_position + (nodal_point - film_position) * nodal_focal_ratio;

    //Get sample position on lens
    Vector2f aperture_sample = rejectionSampling(_random) * (m_focal_length / m_aperture);
    Vector3f aperture_position = (m_right * aperture_sample.x()) + (m_up * aperture_sample.y());

    //Given x and y position between -1 and 1 generate ray postioned on camera plane and calculate direction
    Vector3f position = nodal_point + aperture_position;
    Vector3f direction = (focal_point - position).normalized();

    //Update data
    std::copy(position.data(), position.data() + 3, _ouput[iterator].org);
    std::copy(direction.data(), direction.data() + 3, _ouput[iterator].dir);
  }
}

//Rejection sampling used for creating rays across the lens
Vector2f GeometricCamera::rejectionSampling(RandomGenerator* _random) const
{
  Vector2f output;

  while(true)
  {
    output = Vector2f(_random->getSample() - 0.5f, _random->getSample() - 0.5f);
    if(output.norm() < 0.5f)
      break;
  }

  return output;
}

void GeometricCamera::direction(const Vector3f &_dir)
{
  //Set camera direction and normalize
  m_dir = _dir.normalized();

  //Recalculate right and up vectors
  m_right = Vector3f(0.f, 1.f, 0.f).cross(m_dir).normalized();
  m_up = m_dir.cross(m_right).normalized();
}

MSC_NAMESPACE_END