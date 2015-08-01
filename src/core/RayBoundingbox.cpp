#include <core/RayBoundingbox.h>

MSC_NAMESPACE_BEGIN

void RayBoundingbox::operator()(const tbb::blocked_range< size_t >& r)
{
  size_t begin = r.begin();
  size_t end = r.end();

  m_value.min[0] = m_data[begin].org[0];
  m_value.min[1] = m_data[begin].org[1];
  m_value.min[2] = m_data[begin].org[2];
  m_value.max[0] = m_data[begin].org[0];
  m_value.max[1] = m_data[begin].org[1];
  m_value.max[2] = m_data[begin].org[2];

  for(size_t index = begin; index < end; ++index)
  {
    if(m_data[index].org[0] < m_value.min[0])
      m_value.min[0] = m_data[index].org[0];
    if(m_data[index].org[1] < m_value.min[1])
      m_value.min[1] = m_data[index].org[1];
    if(m_data[index].org[2] < m_value.min[2])
      m_value.min[2] = m_data[index].org[2];

    if(m_data[index].org[0] > m_value.max[0])
      m_value.max[0] = m_data[index].org[0];
    if(m_data[index].org[1] > m_value.max[1])
      m_value.max[1] = m_data[index].org[1];
    if(m_data[index].org[2] > m_value.max[2])
      m_value.max[2] = m_data[index].org[2];
  }
}

void RayBoundingbox::join( RayBoundingbox& rhs )
{
  if(rhs.m_value.min[0] < m_value.min[0])
    m_value.min[0] = rhs.m_value.min[0];
  if(rhs.m_value.min[1] < m_value.min[1])
    m_value.min[1] = rhs.m_value.min[1];
  if(rhs.m_value.min[2] < m_value.min[2])
    m_value.min[2] = rhs.m_value.min[2];

  if(rhs.m_value.max[0] > m_value.max[0])
    m_value.max[0] = rhs.m_value.max[0];
  if(rhs.m_value.max[1] > m_value.max[1])
    m_value.max[1] = rhs.m_value.max[1];
  if(rhs.m_value.max[2] > m_value.max[2])
    m_value.max[2] = rhs.m_value.max[2];
}

MSC_NAMESPACE_END