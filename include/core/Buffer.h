#ifndef _BIN_H_
#define _BIN_H_

#include <vector>

#include <core/Common.h>
#include <core/RayCompressed.h>

MSC_NAMESPACE_BEGIN

/**
 * @brief      Holds local rays as they are produced to minimise thread contention
 */
struct Buffer
{
  // size_t size;
  // size_t index[6];
  std::vector< RayCompressed > direction[6];
};

MSC_NAMESPACE_END

#endif
