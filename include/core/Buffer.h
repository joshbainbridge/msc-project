#ifndef _BIN_H_
#define _BIN_H_

#include <vector>

#include <core/Common.h>
#include <core/RayCompressed.h>

MSC_NAMESPACE_BEGIN

/**
 * @brief      Holds local rays as they are produced to minimise thread contention
 * 
 * Local thread buffer that will collect rays into six directional std::vectors during camera
 * sampling or surface shading. When a camera bucket or shading surface is finished, these will
 * then be loaded into the shared and memory mapped files.
 */
struct Buffer
{
  std::vector< RayCompressed > direction[6];
};

MSC_NAMESPACE_END

#endif
