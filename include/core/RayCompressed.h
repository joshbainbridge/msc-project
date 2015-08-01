#ifndef _RAYCOMPRESSED_H_
#define _RAYCOMPRESSED_H_

#include <core/Common.h>

MSC_NAMESPACE_BEGIN

/**
 * @brief      Structure to contain a compressed ray
 */
struct RayCompressed
{
  float org[3];
  float dir[3];

  int sampleID;
};

MSC_NAMESPACE_END

#endif
