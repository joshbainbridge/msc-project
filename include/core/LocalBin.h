#ifndef _LOCALBIN_H_
#define _LOCALBIN_H_

#include <vector>

#include <core/Common.h>
#include <core/Ray.h>

MSC_NAMESPACE_BEGIN

struct LocalBin
{
  size_t index[6];
  std::vector<Ray> bin[6];
};

MSC_NAMESPACE_END

#endif
