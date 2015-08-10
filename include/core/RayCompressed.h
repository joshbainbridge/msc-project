#ifndef _RAYCOMPRESSED_H_
#define _RAYCOMPRESSED_H_

#include <core/Common.h>

MSC_NAMESPACE_BEGIN

/**
 * @brief      Structure to contain a compressed ray
 * 
 * Minimal data required to store a ray that also represents the last segment of a light path within
 * a scene. This has a direct impact on the size of each batch and as result read/write performance.
 */
struct RayCompressed
{
  float org[3];
  float dir[3];

  float weight[3];
  float lastPdf;
  int rayDepth;
  int sampleID;
};

MSC_NAMESPACE_END

#endif
