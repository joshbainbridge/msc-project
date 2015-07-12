#include <core/GeometricCamera.h>

MSC_NAMESPACE_BEGIN

void GeometricCamera::sample(const int _count, float* _positions, RandomGenerator* _random, Ray* _ouput) const
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
    Vector2f aperture_sample = rejectionSampling(_random) * m_aperture;
    Vector3f aperture_position = (m_right * aperture_sample.x()) + (m_up * aperture_sample.y());

    //Given x and y position between -1 and 1 generate ray postioned on camera plane and calculate direction
    Vector3f position = nodal_point + aperture_position;
    Vector3f direction = (focal_point - position).normalized();

    //Update data
    std::copy(position.data(), position.data() + 3, _ouput[iterator].position);
    std::copy(direction.data(), direction.data() + 3, _ouput[iterator].direction);
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

MSC_NAMESPACE_END