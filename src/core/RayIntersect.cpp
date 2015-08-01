#include <core/RayIntersect.h>

#include <iostream>

MSC_NAMESPACE_BEGIN

void RayIntersect::operator()(const tbb::blocked_range< size_t >& r) const
{
  // Test packing data for sse vectorization 
  for(size_t index = r.begin(); index < r.end(); ++index)
    rtcIntersect(m_scene->rtc_scene, m_data[index].rtc_ray);
}

MSC_NAMESPACE_END