#ifndef _BIN_H_
#define _BIN_H_

#include <vector>

#include <core/Common.h>
#include <core/RayCompressed.h>

MSC_NAMESPACE_BEGIN

struct Buffer
{
  // size_t size;
  // size_t index[6];
  std::vector< RayCompressed > direction[6];
};

MSC_NAMESPACE_END

#endif
