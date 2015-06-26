#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#include <core/Common.hpp>

MSC_NAMESPACE_BEGIN

struct Settings
{
  Settings()
    : resolution_x(0)
    , resolution_y(0)
    , bucket_size(0)
    , frame_number(0)
    , samples(0)
    , depth(0)
    , absorption(0.f)
  {;}
  
  int resolution_x;
  int resolution_y;
  int bucket_size;
  int frame_number;
  int samples;
  int depth;
  float absorption;
};

MSC_NAMESPACE_END

#endif
