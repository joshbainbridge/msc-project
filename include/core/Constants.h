#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

#include <float.h>

#ifdef M_PI
  #undef M_PI
#endif

#define M_PI 3.14159265358979323846f
#define M_INV_PI 0.31830988618379067154f
#define M_PI_180 0.01745329251994329577f
#define M_180_PI 57.2957795130823208768f

#define M_EPSILON FLT_EPSILON
#define M_INFINITY FLT_MAX

#define MSC_NAMESPACE_BEGIN namespace msc {
#define MSC_NAMESPACE_END }

#endif