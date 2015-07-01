#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#include <core/Common.h>

MSC_NAMESPACE_BEGIN

struct Settings
{
  size_t ray_depth;
  size_t bucket_size;
  size_t iteration;
};

MSC_NAMESPACE_END

#endif
