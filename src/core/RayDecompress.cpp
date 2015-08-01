#include <core/RayDecompress.h>

MSC_NAMESPACE_BEGIN

void RayDecompress::operator()(const tbb::blocked_range< size_t >& r) const
{
  for(size_t index = r.begin(); index < r.end(); ++index)
  {
    m_output[index].org[0] = m_input[index].org[0];
    m_output[index].org[1] = m_input[index].org[1];
    m_output[index].org[2] = m_input[index].org[2];
    m_output[index].dir[0] = m_input[index].dir[0];
    m_output[index].dir[1] = m_input[index].dir[1];
    m_output[index].dir[2] = m_input[index].dir[2];
    m_output[index].tnear = 0.001f;
    m_output[index].tfar = 100000.f;
    m_output[index].geomID = RTC_INVALID_GEOMETRY_ID;
    m_output[index].primID = RTC_INVALID_GEOMETRY_ID;
    m_output[index].instID = RTC_INVALID_GEOMETRY_ID;
    m_output[index].time = 0.f;
    m_output[index].sampleID = m_input[index].sampleID;
  }
}

MSC_NAMESPACE_END