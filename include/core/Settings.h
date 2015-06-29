#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#include <core/Common.h>

MSC_NAMESPACE_BEGIN

struct Settings
{
  Settings() : depth(0) {;}
  
  size_t depth;
};

MSC_NAMESPACE_END

#endif
