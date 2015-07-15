#ifndef _RAYUNCOMPRESSED_H_
#define _RAYUNCOMPRESSED_H_

#include <core/EmbreeWrapper.h>
#include <core/Common.h>

MSC_NAMESPACE_BEGIN

struct RayUncompressed
{
  union
  {
    RTCRay4 rtc_ray;
    struct RTCORE_ALIGN(16)
    {
      // Ray data
      float orgx[4];
      float orgy[4];
      float orgz[4];
      
      float dirx[4];
      float diry[4];
      float dirz[4];
      
      float tnear[4];
      float tfar[4];

      float time[4];
      int mask[4];

      //Hit data
      float Ngx[4];
      float Ngy[4];
      float Ngz[4];

      float u[4];
      float v[4];

      int geomID[4];
      int primID[4];
      int instID[4];
    };
  };

  //Added data
  int sampleID[4];
  int32_t valid[4];
};

MSC_NAMESPACE_END

#endif
