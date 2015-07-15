#ifndef _RAYUNCOMPRESSED_H_
#define _RAYUNCOMPRESSED_H_

#include <core/EmbreeWrapper.h>
#include <core/Common.h>

MSC_NAMESPACE_BEGIN

struct RayUncompressed
{
  union
  {
    RTCRay rtc_ray;
    struct RTCORE_ALIGN(16)
    {
      //Ray data
      float org[3];
      float align0;
      
      float dir[3];
      float align1;
      
      float tnear;
      float tfar;

      float time;
      int mask;
      
      // Hit data
      float Ng[3];
      float align2;
      
      float u;
      float v;

      int geomID;
      int primID;
      int instID;
    };
  };

  //Added data
  int sampleID;
};

MSC_NAMESPACE_END

#endif
