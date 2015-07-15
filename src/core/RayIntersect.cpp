#include <core/RayIntersect.h>

#include <iostream>

MSC_NAMESPACE_BEGIN

void RayIntersect::operator()(const Range64< size_t >& r) const
{
  size_t remain = r.size() % 4;
  size_t end = r.end() - remain;

  RTCRay4 ray;
  int32_t mask[4] = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};

  for(size_t iterator = r.begin(); iterator < end; iterator += 4)
  {
    ray.orgx[0] = m_data[iterator + 0].org[0];
    ray.orgy[0] = m_data[iterator + 0].org[1];
    ray.orgz[0] = m_data[iterator + 0].org[2];
    ray.dirx[0] = m_data[iterator + 0].dir[0];
    ray.diry[0] = m_data[iterator + 0].dir[1];
    ray.dirz[0] = m_data[iterator + 0].dir[2];
    ray.tnear[0] = 0.1f;
    ray.tfar[0] = 100000.f;
    ray.geomID[0] = RTC_INVALID_GEOMETRY_ID;
    ray.primID[0] = RTC_INVALID_GEOMETRY_ID;
    ray.instID[0] = RTC_INVALID_GEOMETRY_ID;
    ray.time[0] = 0.f;

    ray.orgx[1] = m_data[iterator + 1].org[0];
    ray.orgy[1] = m_data[iterator + 1].org[1];
    ray.orgz[1] = m_data[iterator + 1].org[2];
    ray.dirx[1] = m_data[iterator + 1].dir[0];
    ray.diry[1] = m_data[iterator + 1].dir[1];
    ray.dirz[1] = m_data[iterator + 1].dir[2];
    ray.tnear[1] = 0.1f;
    ray.tfar[1] = 100000.f;
    ray.geomID[1] = RTC_INVALID_GEOMETRY_ID;
    ray.primID[1] = RTC_INVALID_GEOMETRY_ID;
    ray.instID[1] = RTC_INVALID_GEOMETRY_ID;
    ray.time[1] = 0.f;

    ray.orgx[2] = m_data[iterator + 2].org[0];
    ray.orgy[2] = m_data[iterator + 2].org[1];
    ray.orgz[2] = m_data[iterator + 2].org[2];
    ray.dirx[2] = m_data[iterator + 2].dir[0];
    ray.diry[2] = m_data[iterator + 2].dir[1];
    ray.dirz[2] = m_data[iterator + 2].dir[2];
    ray.tnear[2] = 0.1f;
    ray.tfar[2] = 100000.f;
    ray.geomID[2] = RTC_INVALID_GEOMETRY_ID;
    ray.primID[2] = RTC_INVALID_GEOMETRY_ID;
    ray.instID[2] = RTC_INVALID_GEOMETRY_ID;
    ray.time[2] = 0.f;

    ray.orgx[3] = m_data[iterator + 3].org[0];
    ray.orgy[3] = m_data[iterator + 3].org[1];
    ray.orgz[3] = m_data[iterator + 3].org[2];
    ray.dirx[3] = m_data[iterator + 3].dir[0];
    ray.diry[3] = m_data[iterator + 3].dir[1];
    ray.dirz[3] = m_data[iterator + 3].dir[2];
    ray.tnear[3] = 0.1f;
    ray.tfar[3] = 100000.f;
    ray.geomID[3] = RTC_INVALID_GEOMETRY_ID;
    ray.primID[3] = RTC_INVALID_GEOMETRY_ID;
    ray.instID[3] = RTC_INVALID_GEOMETRY_ID;
    ray.time[3] = 0.f;

    rtcIntersect4(mask, m_scene->rtc_scene, ray);
  }

  if(remain != 0)
  {

  }
  // rtcIntersect(m_scene->rtc_scene, m_rays[iterator]);
  // m_image->samples[] = m_rays[iterator].tnear;
}

MSC_NAMESPACE_END