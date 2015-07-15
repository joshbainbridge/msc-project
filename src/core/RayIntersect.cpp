#include <core/RayIntersect.h>

MSC_NAMESPACE_BEGIN

void RayIntersect::operator()(const tbb::blocked_range< size_t >& r) const
{
  for(size_t iterator = r.begin(); iterator < r.end(); ++iterator)
    rtcIntersect(m_scene->rtc_scene, m_data[iterator].rtc_ray);
}

MSC_NAMESPACE_END