#include <core/CameraInterface.h>

MSC_NAMESPACE_BEGIN

//Set camera direction and update other relevant vectors
void CameraInterface::direction(const Vector3f &_dir)
{
  //Set camera direction and normalize
  m_dir = _dir.normalized();

  //Recalculate right and up vectors
  m_right = Vector3f(0.f, 1.f, 0.f).cross(m_dir).normalized();
  m_up = m_dir.cross(m_right).normalized();
}

MSC_NAMESPACE_END