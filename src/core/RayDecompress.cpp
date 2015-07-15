#include <core/RayDecompress.h>

MSC_NAMESPACE_BEGIN

void RayDecompress::operator()(const tbb::blocked_range< size_t >& r) const
{
  for(size_t iterator = r.begin(); iterator < r.end(); ++iterator)
  {
    m_output[iterator].org[0] = m_input[iterator].org[0];
    m_output[iterator].org[1] = m_input[iterator].org[1];
    m_output[iterator].org[2] = m_input[iterator].org[2];
    m_output[iterator].dir[0] = m_input[iterator].dir[0];
    m_output[iterator].dir[1] = m_input[iterator].dir[1];
    m_output[iterator].dir[2] = m_input[iterator].dir[2];
    m_output[iterator].tnear = 0.001f;
    m_output[iterator].tfar = 100000.f;
    m_output[iterator].geomID = RTC_INVALID_GEOMETRY_ID;
    m_output[iterator].primID = RTC_INVALID_GEOMETRY_ID;
    m_output[iterator].instID = RTC_INVALID_GEOMETRY_ID;
    m_output[iterator].time = 0.f;
    m_output[iterator].sampleID = m_input[iterator].sampleID;
  }
}

MSC_NAMESPACE_END