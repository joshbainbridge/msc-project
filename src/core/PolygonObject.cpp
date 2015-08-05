#include <core/PolygonObject.h>

MSC_NAMESPACE_BEGIN

void PolygonObject::construct()
{
  tinyobj::shape_t shape;
  std::string err = tinyobj::LoadObj(shape, m_filename.c_str());

  // if(!err.empty())
  //   std::cout << err << std::endl;

  m_normals.swap(shape.mesh.normals);
  m_texcoords.swap(shape.mesh.texcoords);
  m_indices.swap(shape.mesh.indices);

  Affine3f transform = Affine3f::Identity()
   * Eigen::Translation3f(m_translation)
   * Eigen::AngleAxisf(m_rotation.x() * M_PI_180, Vector3f::UnitX())
   * Eigen::AngleAxisf(m_rotation.y() * M_PI_180, Vector3f::UnitY())
   * Eigen::AngleAxisf(m_rotation.z() * M_PI_180, Vector3f::UnitZ())
   * Eigen::AlignedScaling3f(m_scale);

  Vector3fMap mapped_normal(NULL);
  for(size_t i = 0; i < m_normals.size() / 3; ++i)
  {
    new (&mapped_normal) Vector3fMap((float*) &(m_normals[3 * i + 0]));
    mapped_normal = (transform.linear().inverse().transpose() * mapped_normal).normalized();
  }

  Vector3fMap mapped_position(NULL);
  m_positions.resize((shape.mesh.positions.size() / 3) * 4);
  for(size_t i = 0; i < shape.mesh.positions.size() / 3; ++i)
  {
    m_positions[4 * i + 0] = shape.mesh.positions[3 * i + 0];
    m_positions[4 * i + 1] = shape.mesh.positions[3 * i + 1];
    m_positions[4 * i + 2] = shape.mesh.positions[3 * i + 2];
    m_positions[4 * i + 3] = 0.f;

    new (&mapped_position) Vector3fMap((float*) &(m_positions[4 * i + 0]));
    mapped_position = transform * mapped_position;
  }
}

void PolygonObject::texture(
  const size_t _primitive,
  const float _s,
  const float _t,
  Vector2f* _output
  ) const
{
  const size_t _index_base = m_indices[3 * _primitive + 0];
  const size_t _index_s = m_indices[3 * _primitive + 1];
  const size_t _index_t = m_indices[3 * _primitive + 2];

  (*_output)[0] = (1.f - _s - _t) * m_texcoords[2 * _index_base + 0] + 
  _s * m_texcoords[2 * _index_s + 0] + 
  _t * m_texcoords[2 * _index_t + 0];

  (*_output)[1] = (1.f - _s - _t) * m_texcoords[2 * _index_base + 1] + 
  _s * m_texcoords[2 * _index_s + 1] + 
  _t * m_texcoords[2 * _index_t + 1];
}

void PolygonObject::normal(
  const size_t _primitive,
  const float _s,
  const float _t,
  Vector3f* _output
  ) const
{
  const size_t _index_base = m_indices[3 * _primitive + 0];
  const size_t _index_s = m_indices[3 * _primitive + 1];
  const size_t _index_t = m_indices[3 * _primitive + 2];

  (*_output)[0] = (1.f - _s - _t) * m_normals[3 * _index_base + 0] + 
  _s * m_normals[3 * _index_s + 0] + 
  _t * m_normals[3 * _index_t + 0];

  (*_output)[1] = (1.f - _s - _t) * m_normals[3 * _index_base + 1] + 
  _s * m_normals[3 * _index_s + 1] + 
  _t * m_normals[3 * _index_t + 1];

  (*_output)[2] = (1.f - _s - _t) * m_normals[3 * _index_base + 2] + 
  _s * m_normals[3 * _index_s + 2] + 
  _t * m_normals[3 * _index_t + 2];
}

MSC_NAMESPACE_END