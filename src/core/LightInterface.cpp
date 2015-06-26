#include <core/LightInterface.h>

MSC_NAMESPACE_BEGIN

//Set light direction and update other relevant vectors
void LightInterface::direction(const Vector3f &_dir)
{
  //Set light direction and normalize
  m_dir = _dir.normalized();

  //Recalculate right and up vectors
  m_right = Vector3f(0.f, 1.f, 0.001f).cross(m_dir).normalized();
  m_up = m_dir.cross(m_right).normalized();
}

MSC_NAMESPACE_END